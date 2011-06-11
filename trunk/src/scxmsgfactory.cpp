#include <QtCore/QDebug>
#include "scxmsgfactory.h"

#define SCX_PROTO_START_HEADER 0x55

SCXMsgFactory::SCXMsgFactory(QObject *parent) :
        QSlotRacingMsgFactory(parent),
        m_statCounters(),
        m_currentMsgIndex(0)
{
    // empty out the current message buffer
    for (quint32 i = 0; i < SCX_PROTO_MAX_MSG_LENGTH; i++)
    {
        m_currentMsg[i] = 0;
    }
    
    // set stat entries' names
    m_statCounters.SetEntryTitle(eStatEntry_BytesProcessedOK,
                                 QString("BytesProcessedOK"));
    m_statCounters.SetEntryTitle(eStatEntry_BytesDiscarded,
                                 QString("BytesDiscarded"));
    m_statCounters.SetEntryTitle(eStatEntry_BytesTotal,
                                 QString("BytesTotal"));
}

SCXMsgFactory::~SCXMsgFactory()
{
}

void SCXMsgFactory::Parse(QByteArray a_dataBuffer)
{
    for (qint32 i = 0; i < a_dataBuffer.count(); i++)
    {
        m_statCounters.Increment(eStatEntry_BytesTotal, 1);
        
        if (m_currentMsgIndex == 0)
        {
            // current message has no data at all
            // look for the start header on current buffer
            if (a_dataBuffer.at(i) == SCX_PROTO_START_HEADER)
            {
                m_currentMsg[m_currentMsgIndex++] = a_dataBuffer.at(i);
                m_statCounters.Increment(eStatEntry_BytesProcessedOK, 1);
            }
            else
            {
                // a msg MUST start with SCX_PROTO_START_HEADER
                m_statCounters.Increment(eStatEntry_BytesDiscarded, 1);
            }
        }
        else
        {
            m_currentMsg[m_currentMsgIndex++] = a_dataBuffer.at(i);
            m_statCounters.Increment(eStatEntry_BytesProcessedOK, 1);
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
