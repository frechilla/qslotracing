#include "scxmsgfactory.h"

#define SCX_PROTO_START_HEADER 0x55

SCXMsgFactory::SCXMsgFactory(QObject *parent) :
        QSlotRacingMsgFactory(parent),
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

void SCXMsgFactory::Parse(QByteArray a_dataBuffer)
{
    for (qint32 i = 0; i < a_dataBuffer.count(); i++)
    {
        if (m_currentMsgIndex == 0)
        {
            // current message has no data at all
            // look for the start header on current buffer
            if (a_dataBuffer.data()[i] == SCX_PROTO_START_HEADER)
            {
                m_currentMsg[m_currentMsgIndex++] = a_dataBuffer.at(i);
            }
            else
            {
                // a msg MUST start with SCX_PROTO_START_HEADER
                m_bytesDiscarded++;
            }
        }
        else
        {
            m_currentMsg[m_currentMsgIndex++] = a_dataBuffer.at(i);
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

            // send this new message somewhere beyond the sea...
            emit MsgParsed(newSlotRacingMsg);
        }        
    }
}
