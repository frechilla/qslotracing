#include "snifferserial.h"
#include <QMutex>
#include <QWaitCondition>

#define READFOREVER_BUFFER_LEN       10
#define READFOREVER_WAIT_MSEC       250
#define READFOREVER_IDLE_WIRE_MSEC 1000 // 1 second

SnifferSerial::SnifferSerial(QObject *parent) :
    QObject(parent),
    m_workerThread(parent),
    m_snifferOn(false),
    m_threadAlive(true),
    m_interfaceOpened(false),
    m_port(0),
    m_interfaceName(""),
    m_baudRate(AbstractSerial::BaudRateUndefined),
    m_dataBits(AbstractSerial::DataBitsUndefined),
    m_parity(AbstractSerial::ParityUndefined),
    m_stopBits(AbstractSerial::StopBitsUndefined),
    m_flowControl(AbstractSerial::FlowControlUndefined)
{
#if 0
    // add PrintBuffer to the list of slots
    this->connect(this, SIGNAL(bytesRead(QByteArray)), SLOT(PrintBuffer(QByteArray)));
#endif

    // this object is going to sniff using a different thread
    // set that up before doing anything else
    this->connect(&m_workerThread, SIGNAL(started()), SLOT(ReadForever()));
    this->moveToThread(&m_workerThread);

    // start the thread now so it is ready to sniff
    m_workerThread.start();
}

SnifferSerial::~SnifferSerial()
{
    // close current serial interface before destroying this instance
    this->Close();
}

void SnifferSerial::Close()
{
    if (m_port != 0)
    {
        delete m_port;
        m_port = 0;
    }
}

void SnifferSerial::SetDeviceName(const QString &a_deviceName)
{
    m_interfaceName = a_deviceName;
}

void SnifferSerial::SetBaudRate(AbstractSerial::BaudRate a_baudRate)
{
    m_baudRate = a_baudRate;
}

void SnifferSerial::SetDataBits(AbstractSerial::DataBits a_dataBits)
{
    m_dataBits = a_dataBits;
}

void SnifferSerial::SetParity(AbstractSerial::Parity a_parity)
{
    m_parity = a_parity;
}

void SnifferSerial::SetStopBits(AbstractSerial::StopBits a_stopBits)
{
    m_stopBits = a_stopBits;
}

void SnifferSerial::SetFlowControl(AbstractSerial::Flow a_flowControl)
{
    m_flowControl = a_flowControl;
}

void SnifferSerial::OpenSerial()
{
    // close current interface before opening a new connection
    this->Close();

    // 1. First - create an instance of an object
    m_port = new AbstractSerial();
    
    // 2. Second - set the device name
    m_port->setDeviceName(m_interfaceName);

    // 3. Third - open the device
    if (m_port->open(AbstractSerial::ReadWrite | AbstractSerial::Unbuffered))
    {
        // Connect for event driven serial port read
        //this->connect(m_port, SIGNAL(readyRead()), SLOT(SlotRead()));

        qDebug() << "Serial device " << m_port->deviceName() << " open in " << m_port->openMode();

        //Here, the current parameters
        qDebug() << "= Current parameters =";
        qDebug() << "Device name            : " << m_port->deviceName();
        qDebug() << "Baud rate              : " << m_port->baudRate();
        qDebug() << "Data bits              : " << m_port->dataBits();
        qDebug() << "Parity                 : " << m_port->parity();
        qDebug() << "Stop bits              : " << m_port->stopBits();
        qDebug() << "Flow                   : " << m_port->flowControl();

        /* 4. Fourth - now you can set the parameters. (after successfully opened port)
        */

        //TODO proper error handling
        if (!m_port->setBaudRate(m_baudRate))
        {
            qDebug() << "Set baud rate " <<  m_baudRate << " error.";
            this->Close();
            return;
        }

        if (!m_port->setDataBits(m_dataBits)) {
            qDebug() << "Set data bits " <<  m_dataBits << " error.";
            this->Close();
            return;
        }

        if (!m_port->setParity(m_parity)) {
            qDebug() << "Set parity " <<  m_parity << " error.";
            this->Close();
            return;
        }

        if (!m_port->setStopBits(m_stopBits)) {
            qDebug() << "Set stop bits " <<  m_stopBits << " error.";
            this->Close();
            return;
        }

        if (!m_port->setFlowControl(m_flowControl)) {
            qDebug() << "Set flow " <<  m_flowControl << " error.";
            this->Close();
            return;
        }

        /*
            ...
            here you can set other parameters.
            ...
        */

        /*
        Important Note:

        1. For All OS:
        If you use buffered mode (ie, at the opening did not put the flag AbstractSerial::Unbuffered),
        there is no need to set timeouts reading (Ie they are to remain the default = 0)/
        Any value other than 0 will only slow down data acquisition.

        2. For Windows:
        If you are using unbuffered mode, the timeouts have the effect of reading!
        Necessary for the total timeout to set the value of reading At least 1 ms,
        or (at 0) will not be read.

        PS: I have not figured out yet what the reason.

        3. For *.nix:
        If you are using unbuffered mode, the timeouts have the effect of reading!
        Necessary for the total timeout to set the value of reading At least 100 ms,
        as if the value is 0 read will return immediately,
        so you can not read the requested number of bytes (ie, reading function can return fewer bytes).

        In any case, experiment with options for treatment with buffered/unbuffered,
        as well as the timeout values from 0 to N and find the differences. :)
       */

        // Here set total timeout for read 1 ms, if open mode is Unbuffered only!
#if defined (Q_OS_UNIX)
        // Method setTotalReadConstantTimeout() not supported in *.nix.
        if (m_port->openMode() & AbstractSerial::Unbuffered)
        {
            m_port->setCharIntervalTimeout(5000);//5 msec
        }
#elif defined (Q_OS_WIN)
        if (m_port->openMode() & AbstractSerial::Unbuffered)
        {
            m_port->setTotalReadConstantTimeout(100);
            qDebug() << "Running in WINDOWS";
        }
#endif

        // notify the thread we have a valid serial interface opened
        m_interfaceOpened = true;

        //Here, the new set parameters (for example)
        qDebug() << "= New parameters =";
        qDebug() << "Device name            : " << m_port->deviceName();
        qDebug() << "Baud rate              : " << m_port->baudRate();
        qDebug() << "Data bits              : " << m_port->dataBits();
        qDebug() << "Parity                 : " << m_port->parity();
        qDebug() << "Stop bits              : " << m_port->stopBits();
        qDebug() << "Flow                   : " << m_port->flowControl();
    }
    else
    {
        qDebug() << "Error opening serial device " << m_port->deviceName();
    }
}

void SnifferSerial::Pause()
{
    // atomically set snifferOn to false
    m_snifferOn = false;
}

void SnifferSerial::Resume()
{
    // atomically set snifferOn to true
    m_snifferOn = true;
}

void SnifferSerial::Join()
{
    // Tell the worker thread to kill itself by atomically
    // setting the threadAlive variable to false
    m_threadAlive = false;

    // tell the worker thread to exit the QT event loop
    m_workerThread.exit();

    // wait now until the thread is dead
    m_workerThread.wait();
}

void SnifferSerial::ReadForever()
{
    //TODO hacky way to sleep
    QMutex mutex;
    QWaitCondition sleep;

    quint32 idleTime = 0;
    bool    idleWire = false;
    while (m_threadAlive)
    {
        if (m_interfaceOpened && m_snifferOn && m_port)
        {
            if ((m_port->bytesAvailable() > 0) ||
                 m_port->waitForReadyRead(READFOREVER_WAIT_MSEC))
            {
                // reset idleTime
                idleTime = 0;
                idleWire = false;
                
                QByteArray data(READFOREVER_BUFFER_LEN + 1, '\0');
                data = m_port->read(READFOREVER_BUFFER_LEN);

                emit DataRead(data);
            }
            else
            {
                idleTime += READFOREVER_WAIT_MSEC;
                
                if (!idleWire && (idleTime > READFOREVER_IDLE_WIRE_MSEC))
                {
                    // idle wire detected!
                    idleWire = true;
                    emit IdleWire();
                }
            }
        }
        else
        {
            //TODO hacky way to sleep.
            // Proper way would be to extend this class from QThread
            // sleep for READFOREVER_WAIT_MSEC ms
            mutex.lock();
            sleep.wait(&mutex, READFOREVER_WAIT_MSEC);
            mutex.unlock();
        }
    }
}

void SnifferSerial::SlotRead()
{
    QByteArray ba;
    ba = m_port->readAll();

    qDebug()<<"####["<<ba.size()<<"]";

    // send bytes read to upper layers
    emit(DataRead(ba));
}

void SnifferSerial::PrintBuffer(QByteArray ba)
{
    qDebug() << "Read is : " << ba.size() << " bytes:";
    for (qint32 i = 0; i < ba.size(); i++)
    {
        qDebug() << ba.at(i);
    }
}

void SnifferSerial::Write()
{
    QByteArray ba; //data to send
    qint64 bw = 0; //bytes really writed

    ba.resize(4);
    ba[0] = 'a';
    ba[1] = 'b';
    ba[2] = 'c';
    ba[3] = 'd';
    bw = 4;

    bw = m_port->write(ba);
    qDebug() << "Written is : " << bw << " bytes";
}
