#ifndef QSLOTRACINGMSGFACTORY_H
#define QSLOTRACINGMSGFACTORY_H

#include <QSharedPointer> // wrap QSlotRacingMsg objects and share them accross
                          // several threads
#include "qslotracingmsg.h"
#include "delegate.h"

/// @brief vitual class to be inherited by all factories of QSlotRacingMsgs
/// Children classes whould try to separate the flow of input data into separate
/// protocol units (QSlotRacinsgMsg) based on different digital slot car racing
/// protocols
class QSlotRacingMsgFactory
{
public:
    /// @brief type of the delegate where new messages are sent
    typedef Delegate< void(QSharedPointer<QSlotRacingMsg>) > QSlotRacingMsgDelegate_t;

    QSlotRacingMsgFactory() :
            m_msgDelegate() // null delegate for now
    {}
    virtual ~QSlotRacingMsgFactory()
    {}

    /// @brief parses a buffer of bytes of size 'a_bufferSize'
    /// its output is one or more new QSlotRacingMsg object(s)
    /// @param a_dataBuffer points to the byte buffer to be parsed
    /// @param a_bufferSize number of bytes contained in 'a_dataBuffer'
    virtual void Parse(const quint8* a_dataBuffer, quint32 a_bufferSize) = 0;

    /// @return number of bytes discarded in the parsing process
    virtual quint32 GetBytesDiscardedCount() = 0;

    /// @brief sets the function delegate which will be processing the new messages
    /// @param delegate to the function
    void SetMessageProcessorDelegate(
            QSlotRacingMsgDelegate_t a_msgDelegate)
    {
        m_msgDelegate = a_msgDelegate;
    }

protected:
    /// @brief function delegate where new messages will be sent
    QSlotRacingMsgDelegate_t m_msgDelegate;
};

#endif // QSLOTRACINGMSGFACTORY_H
