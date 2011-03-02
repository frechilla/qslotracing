#include "snifferserial.h"

SnifferSerial::SnifferSerial(QObject *parent) :
    QObject(parent),
    m_port(0),
    m_interfaceName(""),
    m_baudRate(AbstractSerial::BaudRateUndefined),
    m_dataBits(AbstractSerial::DataBitsUndefined),
    m_parity(AbstractSerial::ParityUndefined),
    m_stopBits(AbstractSerial::StopBitsUndefined),
    m_flowControl(AbstractSerial::FlowControlUndefined)
{
    // set the prpcessor delegate to PrintBuffer by default
    SetProcessorDelegate(MakeDelegate(&SnifferSerial::PrintBuffer, this));
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

void SnifferSerial::SetProcessorDelegate(QSnifferDelegate_t a_delegate)
{
    m_processorDelegate = a_delegate;
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
        connect(m_port, SIGNAL(readyRead()), this, SLOT(slotRead()));

        qDebug() << "Serial device " << m_port->deviceName() << " open in " << m_port->openMode();

        //Here, the default current parameters (for example)
        qDebug() << "= Default parameters =";
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
        }

        if (!m_port->setDataBits(m_dataBits)) {
            qDebug() << "Set data bits " <<  m_dataBits << " error.";
            this->Close();
        }

        if (!m_port->setParity(m_parity)) {
            qDebug() << "Set parity " <<  m_parity << " error.";
            this->Close();
        }

        if (!m_port->setStopBits(m_stopBits)) {
            qDebug() << "Set stop bits " <<  m_stopBits << " error.";
            this->Close();
        }

        if (!m_port->setFlowControl(m_flowControl)) {
            qDebug() << "Set flow " <<  m_flowControl << " error.";
            this->Close();
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
            qDebug() << "Estamos en WINDOWS";
        }
#endif

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

void SnifferSerial::slotRead()
{
    QByteArray ba;
    ba = m_port->readAll();    
    
    // send bytes read to upper layers
    m_processorDelegate(reinterpret_cast<const quint8*>(ba.data()), ba.size());
}

void SnifferSerial::PrintBuffer(const quint8* a_buffer, quint32 a_bufferSize)
{
    qDebug() << "Readed is : " << a_bufferSize << " bytes:";
    for (quint32 i = 0; i < a_bufferSize; i++)
    {
        qDebug() << a_buffer[i];
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
    qDebug() << "Writed is : " << bw << " bytes";
}