#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QThread>
#include <abstractserial.h>


/// @brief class which porpuse is to retrieve bytes off a serial interface
/// it creates an extra thread to read bytes from the serial interface and
/// output them as a buffer of bytes
class SnifferSerial : public QObject
{
    Q_OBJECT
public:

    explicit SnifferSerial(QObject *parent = 0);
    /// @brief destroys current instance
    /// closes internally the currently opened serial interface
    virtual ~SnifferSerial();
        
    /// @brief opens the serial interface
    /// All parameters must have been set using the "setters" procedures 
    /// before calling to this function
    void OpenSerial();
    
    /// @brief sets the name of the serial interface to be opened
    void SetDeviceName(const QString &a_deviceName);
    
    /// @brief sets baud rate which will be used to open the serial interface
    void SetBaudRate(AbstractSerial::BaudRate a_baudRate);
    
    /// @brief sets data bits which will be used to open the serial interface
    void SetDataBits(AbstractSerial::DataBits a_dataBits);
    
    /// @brief sets parity which will be used to open the serial interface
    void SetParity(AbstractSerial::Parity a_parity);
    
    /// @brief sets stop bits which will be used to open the serial interface
    void SetStopBits(AbstractSerial::StopBits a_stopBits);
    
    /// @brief sets flow control which will be used to open the serial interface
    void SetFlowControl(AbstractSerial::Flow a_flowControl);
    
    /// @brief pause sniffing on the serial interface
    /// This method can be run from a different thread
    void Pause();

    /// @brief resume sniffing on the serial interface
    /// This method can be run from a different thread
    ///
    /// Note you should call to OpenSerial before calling to this method for
    /// the first time. No data will be sniffed until a serial interface is opened
    void Resume();

    /// @brief stops the sniffing thread and waits for the thread to exit
    /// WARNING: This is a blocking call. It won't return until the working thread
    /// exits the "working" loop
    void Join();

    //TODO this is used exclusively for debug. Should be gone at some point
    void Write();

private:
    /// @brief this object's worker thread. It will be running ReadForever
    QThread m_workerThread;

    /// @brief set to true when the sniffer thread is on
    volatile bool m_snifferOn;

    /// @brief true until we want this thread to terminate
    volatile bool m_threadAlive;

    /// @brief set to true when a serial interface has been opened by OpenSerial
    volatile bool m_interfaceOpened;

    /// @brief the serial interface pointer wrapped up with this class
    AbstractSerial* m_port;
    
    /// @brief name of the serial interface. Empty string by default
    QString m_interfaceName;
    
    /// @brief baud rate. undefined by default
    AbstractSerial::BaudRate m_baudRate;
    
    /// @brief data bits. Undefined by default
    AbstractSerial::DataBits m_dataBits;
    
    /// @brief parity. Undefined by default
    AbstractSerial::Parity m_parity;
    
    /// @brief stop bits. Undefined by default
    AbstractSerial::StopBits m_stopBits;
    
    /// @brief flow control. Undefined by default
    AbstractSerial::Flow m_flowControl;

    /// @brief closes the currently opened serial interface
    /// if no interface has been opened yet it does nothing
    void Close();
    
signals:
    /// @brief a singal sent when data has been sniffed off the serial interface
    void DataRead(QByteArray ba);
    
    /// @brief a signal sent when no data has been received for more than 1 second
    void IdleWire();

private slots:
    /// @brief keep this thread stuck in an infinite loop
    /// Sends a DataRead signal every 10 bytes sniffed
    /// Is is used as the slot for the QThread's 'started' signal
    void ReadForever();

    /// @brief this is the handler called whenever data is received on the
    ///        serial interface
    ///
    /// If the serial interface is driven by events it should be connected
    /// to the signal 'readyRead' of the serial monitor
    void SlotRead(void);

    /// @brief prints the buffer received as parameter
    /// it can be used as a dummy slot to print the sniffed stuff
    /// @param data buffer
    void PrintBuffer(QByteArray);
};

#endif // SERIAL_H
