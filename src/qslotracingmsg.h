#ifndef QSLOTRACINGMSG_H
#define QSLOTRACINGMSG_H

#include <QVector>
#include <QTime>
#include <QtGlobal> // Q_ASSERT

/// @brief Class to wrap messages retrieved off the wire (or emulator)
/// it is intended to be as much abstract as possible to allow it on any
/// interface/protocol
/// On construction this gets system time and saves it in an internal timestamp
class QSlotRacingMsg
{
public:
    /// @brief container used to wrap up data messages
    typedef QVector<quint8> QSlotRacingMsgContainer_t;

    /// @brief builds up a new QSlotRacingMsg of a_msgSize bytes
    /// @param a_msgSize size of the message wrapped by this object
    /// @param a_msgBuffer buffer which will be copied inside this object
    ///        it MUST have a_msgSize elements
    QSlotRacingMsg(quint32 a_msgSize, quint8* a_msgBuffer);
    virtual ~QSlotRacingMsg();

    /// @brief writes a_newByte at position a_index
    /// @param new byte to be written
    /// @param index where 'a_newByte' will be written to
    void SetAt(quint8 a_newByte, quint32 a_index);

    /// @return size of the message contained in this obj
    inline qint32 GetMsgSize() const
    {
        return m_data.size();
    }

    /// @return const reference to the message contained in this obj
    /// see http://doc.qt.nokia.com/stable/qvector.html for operations
    /// allowed to be performed on this object.
    /// Read operations can be done with operator[], for instance:
    ///     std::cout << aMsg.GetMsg()[2];
    inline const QSlotRacingMsgContainer_t& GetMsg() const
    {
        return m_data;
    }

    /// @return buffer of the data contained in this message in the old C array way
    inline const quint8* GetBuffer() const
    {
        return m_data.constData();
    }

    /// @return const reference to the creation time of this object
    inline const QTime& GetTimestamp() const
    {
        return m_timestamp;
    }

private:
    /// @brief the data container
    QSlotRacingMsgContainer_t m_data;

    /// @brief timestamp associated with this message
    QTime m_timestamp;

    /// @brief size of the message contained in this obj
    /// @brief private constructor
    /// Prevents this class to be instantiated without the proper arguments
    QSlotRacingMsg();
};

#endif // QSLOTRACINGMSG_H
