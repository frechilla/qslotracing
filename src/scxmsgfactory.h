#ifndef SCXMSGFACTORY_H
#define SCXMSGFACTORY_H

#include "qslotracingmsgfactory.h"
#include "qslotracingstatscounter.h"

// from http://eng.slotbaer.de/SCX/DatProt.html
//     "each datpacket consits of 9 bytes.
//      Of these 3 bytes have a fixed meaning"
#define SCX_PROTO_MSG_LENGTH 9

// just 1 byte is used as CRC
#define SCX_PROTO_CRC_LENGTH 1


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
        eStatEntry_BadCRC,
        
        eStatEntry_Count // this value MUST be at the end of the enum
    } eStatEntries_t;

    // stat counter type
    typedef QSlotRacingStatsCounter<static_cast<int>(eStatEntry_Count)> SCXMsgFactoryStatCounter_t;

    
    SCXMsgFactory(QObject *parent = 0);
    virtual ~SCXMsgFactory();

    /// @brief parses a buffer of bytes of size 'a_bufferSize'
    /// It might trigger one or more new QSlotRacingMsg object(s)
    /// emitting the MsgParsed signal(s)
    /// @param a_dataBuffer byte buffer to be parsed
    void Parse(QByteArray a_dataBuffer);

    /// @return the stat counter of this particular object
    const SCXMsgFactoryStatCounter_t& GetStatCounters() const
    {
        return m_statCounters;
    }

private:
    /// @brief this object's stat counters
    SCXMsgFactoryStatCounter_t m_statCounters;
    
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
    /// @param number of bytes of buffer
    unsigned char CalculateCRC(unsigned char* buffer, int count);

    /// @brief CRC table for calculation
    unsigned char m_crcTable[256];
};

#endif // SCXMSGFACTORY_H
