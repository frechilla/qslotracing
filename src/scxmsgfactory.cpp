#include "scxmsgfactory.h"

#define SCX_PROTO_START_HEADER 0x55

SCXMsgFactory::SCXMsgFactory() :
        QSlotRacingMsgFactory(),
        m_currentMsgIndex(0),
        m_bytesDiscarded(0)
{
    // empty out the current message buffer
    for (quint32 i = 0; i < SCX_PROTO_MAX_MSG_LENGTH; i++)
    {
        m_currentMsg[i] = 0;
    }
}

SCXMsgFactory::~SCXMsgFactory()
{
}

quint32 SCXMsgFactory::GetBytesDiscardedCount()
{
    return m_bytesDiscarded;
}

void SCXMsgFactory::Parse(quint8* a_dataBuffer, quint32 a_bufferSize)
{
    for (quint32 i = 0; i < a_bufferSize; i++)
    {
        if (m_currentMsgIndex == 0)
        {
            // current message has no data at all
            // look for the start header on current buffer
            if (a_dataBuffer[i] == SCX_PROTO_START_HEADER)
            {
                m_currentMsg[m_currentMsgIndex++] = a_dataBuffer[i];
            }
            else
            {
                // a msg MUST start with SCX_PROTO_START_HEADER
                m_bytesDiscarded++;
            }
        }
        else
        {
            m_currentMsg[m_currentMsgIndex++] = a_dataBuffer[i];
        }

        // check if we already built up a message
        if (m_currentMsgIndex == SCX_PROTO_MAX_MSG_LENGTH)
        {
            // create a new QSlotMsg.
            // Timestamp of this message is set at construction time
            QSharedPointer<QSlotRacingMsg> newSlotRacingMsg(
                    new QSlotRacingMsg(SCX_PROTO_MAX_MSG_LENGTH, m_currentMsg));

            // restart message index counter
            m_currentMsgIndex = 0;

            Q_ASSERT(m_msgDelegate);

            // send this new message somewhere beyond the sea...
            m_msgDelegate(newSlotRacingMsg);
        }        
    }
}
