#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <abstractserial.h>


/// @brief class which porpuse is to retrieve bytes off a serial interface
/// it reads bytes from the serial interface and outputs them as a buffer of bytes
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
    
    /// @brief closes the currently opened serial interface
    /// if no interface has been opened yet it does nothing
    void Close();
    
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
    

    //TODO this is used exclusively for debug. Should be gone at some point
    void Write();

private:
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
    
signals:
    void bytesRead(QByteArray ba);

private slots:
    /// @brief this is the handler called whenever data is received on the
    ///        serial interface
    void slotRead(void);

    /// @brief prints the buffer received as parameter
    /// it can be used as a dummy slot to print the sniffed stuff
    /// @param data buffer
    void PrintBuffer(QByteArray);
};

#endif // SERIAL_H
