#ifndef SCXMSGFACTORY_H
#define SCXMSGFACTORY_H

#include "qslotracingmsgfactory.h"

// from http://eng.slotbaer.de/SCX/DatProt.html
//     "each datpacket consits of 9 bytes.
//      Of these 3 bytes have a fixed meaning"
#define SCX_PROTO_MAX_MSG_LENGTH 9

/// @brief class which porpuse is parse data bytes to create new QSlotRacingMsg
/// tries to separate the flow of input data into separate protocol units
/// (QSlotRacinsgMsg) based on the SCX digital protocol as described in
/// http://eng.slotbaer.de/SCX/index.html
class SCXMsgFactory : public QSlotRacingMsgFactory
{
public:
    SCXMsgFactory();
    virtual ~SCXMsgFactory();

    /// @brief parses a buffer of bytes of size 'a_bufferSize'
    /// It might trigger one or more new QSlotRacingMsg object(s)
    /// @param a_dataBuffer points to the byte buffer to be parsed
    /// @param a_bufferSize number of bytes contained in 'a_dataBuffer'
    void Parse(const quint8* a_dataBuffer, quint32 a_bufferSize);

    /// @return number of bytes discarded in the parsing process
    quint32 GetBytesDiscardedCount();

private:
    /// @brief current SCX message
    quint8 m_currentMsg[SCX_PROTO_MAX_MSG_LENGTH];

    /// @brief index where the next byte will be stored in the current SCX message
    quint32 m_currentMsgIndex;

    /// @brief number of bytes discarded in the parsing process
    quint32 m_bytesDiscarded;

    /// @brief wraps up m_currentMsg into a QSlotRacingMsg
    /// It also sends it wherever is expected to
    void CreateNewQSlotRacingMsg();
};

#endif // SCXMSGFACTORY_H
