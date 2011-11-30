#include "scxmsgfactory.h"
#include <QtCore/QDebug>

// SCX proto header. All SCX messages start with this byte
#define SCX_PROTO_START_HEADER 0x55

// undef this variable if there is no trailer at the end of each message
#define SCX_PROTO_TRAILER      0x05

// just 1 byte is used as CRC
#define SCX_PROTO_CRC_LENGTH 1

// position of the CRC byte
#define SCX_PROTO_CRC_POS    8

// Minimun number of valid messages read before asserting communication
#define MIN_VALID_MSGS      10

SCXMsgFactory::SCXMsgFactory(QObject *parent) :
        QSlotRacingMsgFactory(parent),
        m_statCounters(),
        m_currentMsgIndex(0),
        m_nValidMessages(0) // number of valid messages read in a row
{
    // initialise CRC members
    BuildTableCRC();

    // Empty out the current message buffer
    for (quint32 i = 0; i < SCX_PROTO_MSG_LENGTH; i++)
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
    m_statCounters.SetEntryTitle(eStatEntry_MsgDispatched,
                                 QString("MsgDispatched"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgBadCRC,
                                 QString("MsgBadCRC"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgDiscardedNoSync,
                                 QString("MsgDiscardedNoSync"));
    m_statCounters.SetEntryTitle(eStatEntry_ProtoSyncSignalCount,
                                 QString("ProtoSyncSignalCount"));
    m_statCounters.SetEntryTitle(eStatEntry_ProtoUnsyncSignalCount,
                                 QString("ProtoUnsyncSignalCount"));
}

SCXMsgFactory::~SCXMsgFactory()
{
}

void SCXMsgFactory::IdleWire()
{
    //TODO this is debug shite
    qDebug("SCXMsgFactory: Wire has gone idle! Resetting parser...");    
    
    // Restart current message index and valid messages count. 
    // We are no longer in sync, so everything that arrived before
    // will be discarded
    m_currentMsgIndex = 0;
    m_nValidMessages  = 0;
    
    // tell the upper layers we're not in sync anymore
    m_statCounters.Increment(eStatEntry_ProtoUnsyncSignalCount, 1);
    emit ProtocolSynced(false);
}

void SCXMsgFactory::Parse(QByteArray a_dataBuffer)
{
    unsigned char crc;

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

                // Restart current message index and valid messages count. 
                // We are no longer in sync, so everything that arrived before
                // must be discarded
                m_currentMsgIndex = 0;
                m_nValidMessages  = 0;
                
                m_statCounters.Increment(eStatEntry_ProtoUnsyncSignalCount, 1);
                emit ProtocolSynced(false);
            }
        }
        else
        {
            m_currentMsg[m_currentMsgIndex++] = a_dataBuffer.at(i);
            m_statCounters.Increment(eStatEntry_BytesProcessedOK, 1);
        }

        // check if we already built up a message
        if (m_currentMsgIndex == SCX_PROTO_MSG_LENGTH)
        {
            // restart message index counter
            m_currentMsgIndex = 0;

            // Check and increment the number of valid messages read
            if (m_nValidMessages < MIN_VALID_MSGS)
            {
                // Increment number of valid messages
                // No message is passed to upper layers until we reach
                // the confidence level
                m_nValidMessages++;

                // this message is not passed to upper layers.
                // we haven't got to the confidence threshold yet
                m_statCounters.Increment(eStatEntry_MsgDiscardedNoSync, 1);
            }
            else
            {
                if (m_nValidMessages == MIN_VALID_MSGS)
                {
                    // increment number of valid messages so this signal is
                    // only issued once (the first time we get to the minimum
                    // amount of valid messages)
                    m_nValidMessages++;
                    
                    // emit signal. We are in sync
                    m_statCounters.Increment(eStatEntry_ProtoSyncSignalCount, 1);
                    emit ProtocolSynced(true);
                }
                
                // check the CRC first
                crc = CalculateCRC(m_currentMsg, SCX_PROTO_MSG_LENGTH);
                if (crc == m_currentMsg[SCX_PROTO_CRC_POS])
                {
                    m_statCounters.Increment(eStatEntry_MsgDispatched, 1);

                    // create a new QSlotMsg.
                    // Timestamp of this message is set at construction time
                    QSharedPointer<QSlotRacingMsg> newSlotRacingMsg(
                            new QSlotRacingMsg(
                                SCX_PROTO_MSG_LENGTH, 
                                m_currentMsg));

                    // send this new message somewhere beyond the sea...
                    emit MsgParsed(newSlotRacingMsg);
                }
                else
                {
                    // Bad CRC. No need to restart the confidence counter
                    // We discard this message, but we still trust the 
                    // communication
                    m_statCounters.Increment(eStatEntry_MsgBadCRC, 1);

                    ////////////////
                    //TODO this should be gone when the CRC is working correctly
                    qDebug("SCXMsgFactory: Bad CRC %02X (expected %02X)",
                           m_currentMsg[SCX_PROTO_CRC_POS], crc);

                    qDebug("    %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                           m_currentMsg[0],
                           m_currentMsg[1],
                           m_currentMsg[2],
                           m_currentMsg[3],
                           m_currentMsg[4],
                           m_currentMsg[5],
                           m_currentMsg[6],
                           m_currentMsg[7],
                           m_currentMsg[8]);

                    // send this new message somewhere beyond the sea...
                    //QSharedPointer<QSlotRacingMsg> newSlotRacingMsg(
                    //        new QSlotRacingMsg(
                    //            SCX_PROTO_MSG_LENGTH, 
                    //            m_currentMsg));
                    //emit MsgParsed(newSlotRacingMsg);
                    ////////////////
                }
            }
        }
    } // for (qint32 i = 0; i < a_dataBuffer.count(); i++)
}

void SCXMsgFactory::BuildTableCRC()
{
    int i;
    unsigned char table[8];

    table[0] = 0x31;

    for (i = 1; i < 8; i++)
    {
        int n = 2 * table[i - 1];
        table[i] = (n >= 256) ? (n ^ 0x31) : n;
    }

    for (i = 0; i < 256; i++)
    {
        m_crcTable[i] = 0;
        if (i & 1)   m_crcTable[i] ^= table[0];
        if (i & 2)   m_crcTable[i] ^= table[1];
        if (i & 4)   m_crcTable[i] ^= table[2];
        if (i & 8)   m_crcTable[i] ^= table[3];
        if (i & 16)  m_crcTable[i] ^= table[4];
        if (i & 32)  m_crcTable[i] ^= table[5];
        if (i & 64)  m_crcTable[i] ^= table[6];
        if (i & 128) m_crcTable[i] ^= table[7];
    }
}

unsigned char SCXMsgFactory::CalculateCRC(unsigned char* buffer, int count)
{
    int i;
    unsigned char result = 0;

    // from http://eng.slotbaer.de/SCX/DatProt.html
    //   The checksum is calculated over all bytes of the packet, but the checksum
    for (i = 0; i < count - SCX_PROTO_CRC_LENGTH; i++)
    {
        unsigned char idx = buffer[i] ^ result;
        result = m_crcTable[idx];
    }

    return result ^ 0xBB;
}
