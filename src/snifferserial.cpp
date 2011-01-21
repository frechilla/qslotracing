#include "snifferserial.h"

Serial::Serial(QObject *parent) :
    QObject(parent)
{
}

void Serial::OpenSerial()
{
    /* 1. First - create an instance of an object.
    */
    m_port = new AbstractSerial();

    /* 2. Second - set the device name.
    */
    m_port->setDeviceName("COM6");

    /* 3. Third - open the device.
    */
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

        //Here example set baud rate 115200 bit/sec (baud)
        if (!m_port->setBaudRate(AbstractSerial::BaudRate115200))
        {
            qDebug() << "Set baud rate " <<  AbstractSerial::BaudRate115200 << " error.";
            delete m_port;
            m_port = 0;
        }

        if (!m_port->setDataBits(AbstractSerial::DataBits8)) {
            qDebug() << "Set data bits " <<  AbstractSerial::DataBits8 << " error.";
            delete m_port;
            m_port = 0;
        }

        if (!m_port->setParity(AbstractSerial::ParityNone)) {
            qDebug() << "Set parity " <<  AbstractSerial::ParityNone << " error.";
            delete m_port;
            m_port = 0;
        }

        if (!m_port->setStopBits(AbstractSerial::StopBits1)) {
            qDebug() << "Set stop bits " <<  AbstractSerial::StopBits1 << " error.";
            delete m_port;
            m_port = 0;
        }

        if (!m_port->setFlowControl(AbstractSerial::FlowControlOff)) {
            qDebug() << "Set flow " <<  AbstractSerial::FlowControlOff << " error.";
            delete m_port;
            m_port = 0;
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
        qDebug() << "Error opened serial device " << m_port->deviceName();
    }
}

void Serial::Write()
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

void Serial::slotRead()
{
    QByteArray ba;
    quint8 i, tam;

    ba = m_port->readAll();
    tam = ba.size();
    qDebug() << "Readed is : " << ba.size() << " bytes";
    qDebug() << "Leido: ";
    for (i=0;i<tam;i++)
    {
        qDebug() << ba[i];
    }
}
