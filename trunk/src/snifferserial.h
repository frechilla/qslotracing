#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <abstractserial.h>
#include "delegate.h" // delegate of the upper layer that will process
                      // bytes sniffed by this class

/// @brief class which porpuse is to retrieve bytes off a serial interface
/// it reads bytes from the serial interface and outputs them as a buffer of bytes
class SnifferSerial : public QObject
{
    Q_OBJECT
public:
    /// @brief type of the delegate where bytes sniffed are sent
    typedef Delegate< void(const quint8*, quint32) > QSnifferDelegate_t;
    

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
    
    /// @brief adds a_delegate to the list of processing delegates
    /// All delegates in the list will be called per every byte sniffed by this class
    void AddProcessorDelegate(QSnifferDelegate_t a_delegate);
    

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
    
    /// @brief list of delegates where new messages will be sent
    /// it is empty by default so the function AddProcessorDelegate
    /// must be called at least once
    ///
    /// For debugging purposes you might want to add this->PrintBuffer
    /// to ths list
    QList<QSnifferDelegate_t> m_processorDelegateList;
    
    
    /// @brief prints the buffer received as parameter
    /// it can be used as a dummy delegate when data sniffed is not expected
    /// to be processed by anyone else than this funcion
    /// @param pointer to the buffer
    /// @param number of bytes contained in the buffer
    void PrintBuffer(const quint8* a_buffer, quint32 a_bufferSize);

    /// @brief calls all delegates in m_processorDelegateList
    /// all delegates will be called with parameters a_buffer and a_bufferSize
    inline void NotifyDelegates(const quint8* a_buffer, quint32 a_bufferSize)
    {
        QList<QSnifferDelegate_t>::const_iterator it;
        for (it = m_processorDelegateList.begin();
             it != m_processorDelegateList.end();
             it++)
        {
            (*it)(a_buffer, a_bufferSize);
        }
    }

signals:

public slots:
    /// @brief this is the handler called whenever data is received on the
    ///        serial interface
    void slotRead(void);
};

#endif // SERIAL_H
