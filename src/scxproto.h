#ifndef SCXPROTO_H
#define SCXPROTO_H

#include <QObject>
#include <QSharedPointer>
#include "qslotracingmsg.h"
#include "qslotracingevent.h"
#include "qslotracingstatscounter.h"

/// @brief class with a few methods to parse SCX messages
/// based on the info found in:
///     http://eng.slotbaer.de/SCX/DatProt.html
class SCXProtoAnalyzer : public QObject
{
    Q_OBJECT
public:

    /// @brief statistics counter
    typedef enum
    {
        eStatEntry_MsgTotal = 0, // first enum element MUST be set to 0

        eStatEntry_MsgTypeController,
        eStatEntry_MsgTypeId,
        eStatEntry_MsgTypeBusClearance,
        eStatEntry_MsgTypeFinishLine,
        eStatEntry_MsgTypeRanking,
        eStatEntry_MsgTypeLapTime,
        eStatEntry_MsgTypeLapCounter,
        eStatEntry_MsgTypeQualifying,
        eStatEntry_MsgTypeReset,
        eStatEntry_MsgTypeStart,
        eStatEntry_MsgTypeEnd,
        eStatEntry_MsgTypeFuel,
        eStatEntry_MsgTypeRefreshDisplay,
        eStatEntry_MsgTypeBrake,

        eStatEntry_MsgBadType,
        eStatEntry_MsgBadHeader,

        eStatEntry_Count // this value MUST be at the end of the enum
    } eStatEntries_t;

    /// @brief stat counter type
    typedef QSlotRacingStatsCounter<static_cast<int>(eStatEntry_Count)> SCXProtoStatCounter_t;


    /// @brief types of SCX messages
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

    /// @brief SCXProtoAnalyzer class constructor
    SCXProtoAnalyzer(QObject *parent = 0);

    /// @brief SCXProtoAnalyzer class destructor
    virtual ~SCXProtoAnalyzer();

    /// @return the stat counter of this particular object
    const SCXProtoStatCounter_t& GetStatCounters() const
    {
        return m_statCounters;
    }

private:
    /// @brief this object's stat counters
    SCXProtoStatCounter_t m_statCounters;

    /// @brief Method for controller message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgController(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for car Id message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgId(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Bus Clearance message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgBusClearance(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Finish Line message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgFinishLine(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Ranking message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgRanking(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Lap Time message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgLapTime(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Lap Counter message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgLapCounter(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Qualifying message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgQualifying(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Reset message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgReset(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Start message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgStart(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for End message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgEnd(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Fuel message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgFuel(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Refresh Display message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgRefreshDisplay(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

    /// @brief Method for Brake message processing
    /// @param Message data bytes
    /// @param Message data structur
    void ProcessMsgBrake(
            const quint8* a_pData,
            const QSharedPointer<QSlotRacingMsg> &a_msg);

signals:
    void ProtoEvent(QSharedPointer<QSlotRacingEvent>);

public slots:
    /// @brief process a QSlotRacingMsg assuming it is part of the SCX protocol
    /// It will notify to whoever is listening what "events" this message
    /// triggers (they might be 0 or more)
    /// it emits a "ProtoEvent" signal per event to notify the listeners
    /// @param a_msg the message
    void ProcessMsg(QSharedPointer<QSlotRacingMsg> a_msg);

}; // class SCXProtoAnalyzer

#endif // SCXPROTO_H
