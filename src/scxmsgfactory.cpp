#include <QtCore/QDebug>
#include "scxmsgfactory.h"

// SCX proto header. All SCX messages start with this byte
#define SCX_PROTO_START_HEADER 0x55

// undef this variable if there is no trailer at the end of each message
#define SCX_PROTO_TRAILER      0x05

// Minimun number of valid messages read before asserting communication
#define MIN_VALID_MSGS          10

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

    // Initialize number of valid messages to be read
    m_nValidMessages = 0;
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
#ifdef SCX_PROTO_TRAILER
            else if (a_dataBuffer.at(i) == SCX_PROTO_TRAILER)
            {
                // Do not do anything. Just consume this byte
                // so the valid messages count is not restarted
            }
#endif
            else
            {
                // a msg MUST start with SCX_PROTO_START_HEADER
                m_statCounters.Increment(eStatEntry_BytesDiscarded, 1);

                // Restart valid messages count
                m_nValidMessages = 0;
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

            // Check and increment the number of valid messages read
            if (m_nValidMessages < MIN_VALID_MSGS)
            {
                // Increment number of valid messages
                m_nValidMessages++;
            }
            else if (m_nValidMessages == MIN_VALID_MSGS)
            {
                // emit signal
                emit ProtocolSynced();
            }
        }        
    }
}
