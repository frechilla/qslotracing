#ifndef SCXPROTO_H
#define SCXPROTO_H

#include <QSharedPointer>
#include "qslotracingmsg.h"

/// @brief namespace with a few methods to parse SCX messages
/// based on the info found in:
///     http://eng.slotbaer.de/SCX/DatProt.html
namespace scxproto
{

typedef enum
{
    e_SCXMsgTypeController     = 0xff, // tells the car what speed to set and whether the lane change switch is pressed
    e_SCXMsgTypeId             = 0xcc, // The data packet initiates the setting of the id
    e_SCXMsgTypeBusClearance   = 0xaa, // When a car passes the reed switch of the pit stop or start track this packet will be sent twice
    e_SCXMsgTypeFinishLine     = 0xee, // identify the cars which crossed the finish line
    e_SCXMsgTypeRanking        = 0xd3, // This packet informs about the cars current ranking
    e_SCXMsgTypeLapTime        = 0xd4, // number of crossings of the finish line and the time for the last lap
    e_SCXMsgTypeLapCounter     = 0xd5, // startnumber of laps to run and the direction of lap counting
    e_SCXMsgTypeQualifying     = 0xdb, // Marks a qualifing
    e_SCXMsgTypeReset          = 0xd0, // When you press the start key long enpugh this packet will be sent twice
    e_SCXMsgTypeStart          = 0xdd, // will be sent twice after the second resetpacket has been sent
    e_SCXMsgTypeEnd            = 0xdc, // finish line or race aborted
    e_SCXMsgTypeFuel           = 0xd6, // fuel level
    e_SCXMsgTypeRefreshDisplay = 0xde, // tels display devices to refresh their display
    e_SCXMsgTypeBrake          = 0xd7, // only sent to cars on the pit stop

} SCXProtoMsgType_t;

/// @brief process a QSlotRacingMsg assuming it is part of the SCX protocol
/// It will notify to whoever is listening what "events" this message
/// triggers (they might be 0 or more)
/// @param a_msg the message
void ProcessMsg(QSharedPointer<QSlotRacingMsg> a_msg);

void ProcessMsgController(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgId(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgBusClearance(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgFinishLine(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgRanking(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgLapTime(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgLapCounter(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgQualifying(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgReset(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgStart(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgEnd(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgFuel(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgRefreshDisplay(const QSharedPointer<QSlotRacingMsg> &a_msg);
void ProcessMsgBrake(const QSharedPointer<QSlotRacingMsg> &a_msg);

}; // namespace proto

#endif // SCXPROTO_H
