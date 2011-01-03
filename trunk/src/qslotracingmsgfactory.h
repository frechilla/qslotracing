#ifndef QSLOTRACINGMSGFACTORY_H
#define QSLOTRACINGMSGFACTORY_H

#include <QSharedPointer> // wrap QSlotRacingMsg objects and share them accross
                          // several threads
#include "qslotracingmsg.h"

/// @brief vitual class to be inherited by all factories of QSlotRacingMsgs
/// Children classes whould try to separate the flow of input data into separate
/// protocol units (QSlotRacinsgMsg) based on different digital slot car racing
/// protocols
class QSlotRacingMsgFactory
{
public:
    /// @brief type of the function where new messages are sent
    typedef void (*QSlotRacingMsgProcessorFunction_t)(QSharedPointer<QSlotRacingMsg>);

    QSlotRacingMsgFactory() :
            m_msgProcessor(0)
    {}
    virtual ~QSlotRacingMsgFactory()
    {}

    /// @brief parses a buffer of bytes of size 'a_bufferSize'
    /// its output is one or more new QSlotRacingMsg object(s)
    /// @param a_dataBuffer points to the byte buffer to be parsed
    /// @param a_bufferSize number of bytes contained in 'a_dataBuffer'
    virtual void Parse(quint8* a_dataBuffer, quint32 a_bufferSize) = 0;

    /// @return number of bytes discarded in the parsing process
    virtual quint32 GetBytesDiscardedCount() = 0;

    /// @brief sets the function which will be processing the new messages
    /// @param a_msgProcessor pointer to the function
    void SetMessageProcessorFunction(
            QSlotRacingMsgProcessorFunction_t a_msgProcessor)
    {
        m_msgProcessor = a_msgProcessor;
    }

protected:
    /// @brief function pointer where nre messages will be sent
    QSlotRacingMsgProcessorFunction_t m_msgProcessor;
};

#endif // QSLOTRACINGMSGFACTORY_H
