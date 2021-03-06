#ifndef SCXMSGFACTORY_H
#define SCXMSGFACTORY_H

#include "qslotracingmsgfactory.h"
#include "qslotracingstatscounter.h"

// from http://eng.slotbaer.de/SCX/DatProt.html
//     "each datpacket consits of 9 bytes.
//      Of these 3 bytes have a fixed meaning"
#define SCX_PROTO_MSG_LENGTH 9


/// @brief class which porpuse is parse data bytes to create new QSlotRacingMsg
/// tries to separate the flow of input data into separate protocol units
/// (QSlotRacinsgMsg) based on the SCX digital protocol as described in
/// http://eng.slotbaer.de/SCX/index.html
class SCXMsgFactory : public QSlotRacingMsgFactory
{
public:
    // statistics counter
    typedef enum
    {
        eStatEntry_BytesProcessedOK = 0, // first enum element MUST be set to 0
        eStatEntry_BytesDiscarded,
        eStatEntry_BytesTotal,
        eStatEntry_MsgDispatched,
        eStatEntry_MsgBadCRC,
        eStatEntry_MsgDiscardedNoSync,
        eStatEntry_ProtoSyncSignalCount,
        eStatEntry_ProtoUnsyncSignalCount,
        
        eStatEntry_Count // this value MUST be at the end of the enum
    } eStatEntries_t;

    
    SCXMsgFactory(QObject *parent = 0);
    virtual ~SCXMsgFactory();

    /// @brief parses a buffer of bytes of size 'a_bufferSize'
    /// It might trigger one or more new QSlotRacingMsg object(s)
    /// emitting the MsgParsed signal(s)
    /// @param a_dataBuffer byte buffer to be parsed
    void Parse(QByteArray a_dataBuffer);
    
    /// @brief to be called when the wire being sniffed goes idle
    /// resets the communication so that the confidence level must be reached
    /// again before new QSlotRacingMsg objects are amitted
    void IdleWire();

    /// @return the stat counter of this particular object
    const QSlotRacingStatsCounter<static_cast<int>(eStatEntry_Count)>& GetStatCounters() const
    {
        return m_statCounters;
    }

private:
    /// @brief this object's stat counters
    QSlotRacingStatsCounter<static_cast<int>(eStatEntry_Count)> m_statCounters;
    
    /// @brief current SCX message
    quint8 m_currentMsg[SCX_PROTO_MSG_LENGTH];

    /// @brief index where the next byte will be stored in the current SCX message
    quint32 m_currentMsgIndex;

    /// @brief counter of valid messages read
    quint8 m_nValidMessages;


    // CRC calculation functions
    // see http://eng.slotbaer.de/SCX/DatProt.html

    /// @brief This method creates the table for CRC calculation
    void BuildTableCRC();

    /// @brief This method calculates the CRC of the provided buffer
    /// @param input buffer for CRC calculation
    /// @param number of bytes of buffer. It should be set to SCX_PROTO_MSG_LENGTH since
    ///        it doesn't use the last byte of "buffer" (it assumes it contains the CRC)
    unsigned char CalculateCRC(unsigned char* buffer, int count);

    /// @brief CRC table for calculation
    unsigned char m_crcTable[256];
};

#endif // SCXMSGFACTORY_H
