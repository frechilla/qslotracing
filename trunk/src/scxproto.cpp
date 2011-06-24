#include "scxproto.h"
#include <QtCore/QDebug>

// first byte MUST always be 0x55
#define SCX_PROTO_START_HEADER 0x55

// Duplicates MUST be less than DUPLICATE_TIMEOUT_MSEC milliseconds apart
// set to 0 if you want to disable the duplicate checks
#define DUPLICATE_TIMEOUT_MSEC 500 // half a second

SCXProtoAnalyzer::SCXProtoAnalyzer(QObject *parent) :
        QObject(parent),
        m_statCounters(),
        m_lastControllerMsg(),
        m_lastIdMsg(),
        m_lastBusClearanceMsg(),
        m_lastFinishLineMsg(),
        m_lastRankingMsg(),
        m_lastLapTimeMsg(),
        m_lastLapCounterMsg(),
        m_lastQualifyingMsg(),
        m_lastResetMsg(),
        m_lastStartMsg(),
        m_lastEndMsg(),
        m_lastFuelMsg(),
        m_lastRefreshDisplayMsg(),
        m_lastBrakeMsg()
{
    // set stat entries' names
    m_statCounters.SetEntryTitle(eStatEntry_MsgTotal,
                                 QString("MsgTotal"));

    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeController,
                                 QString("MsgTypeController"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeControllerDup,
                                 QString("MsgTypeControllerDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeId,
                                 QString("MsgTypeId"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeIdDup,
                                 QString("MsgTypeIdDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeBusClearance,
                                 QString("MsgTypeBusClearance"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeBusClearanceDup,
                                 QString("MsgTypeBusClearanceDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeFinishLine,
                                 QString("MsgTypeFinishLine"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeFinishLineDup,
                                 QString("MsgTypeFinishLineDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeRanking,
                                 QString("MsgTypeRanking"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeRankingDup,
                                 QString("MsgTypeRankingDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeLapTime,
                                 QString("MsgTypeLapTime"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeLapTimeDup,
                                 QString("MsgTypeLapTimeDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeLapCounter,
                                 QString("MsgTypeLapCounter"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeLapCounterDup,
                                 QString("MsgTypeLapCounterDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeQualifying,
                                 QString("MsgTypeQualifying"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeQualifyingDup,
                                 QString("MsgTypeQualifyingDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeReset,
                                 QString("MsgTypeReset"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeResetDup,
                                 QString("MsgTypeResetDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeStart,
                                 QString("MsgTypeStart"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeStartDup,
                                 QString("MsgTypeStartDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeEnd,
                                 QString("MsgTypeEnd"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeEndDup,
                                 QString("MsgTypeEndDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeFuel,
                                 QString("MsgTypeFuel"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeFuelDup,
                                 QString("MsgTypeFuelDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeRefreshDisplay,
                                 QString("MsgTypeRefreshDisplay"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeRefreshDisplayDup,
                                 QString("MsgTypeRefreshDisplayDup"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeBrake,
                                 QString("MsgTypeBrake"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgTypeBrakeDup,
                                 QString("MsgTypeBrakeDup"));

    m_statCounters.SetEntryTitle(eStatEntry_MsgBadHeader,
                                 QString("MsgBadHeader"));
    m_statCounters.SetEntryTitle(eStatEntry_MsgBadType,
                                 QString("MsgBadType"));
}

SCXProtoAnalyzer::~SCXProtoAnalyzer()
{
}

void SCXProtoAnalyzer::ProcessMsg(QSharedPointer<QSlotRacingMsg> a_msg)
{
    // new message has arrived
    m_statCounters.Increment(eStatEntry_MsgTotal, 1);

    const quint8* pData = a_msg->GetBuffer();

    // see http://eng.slotbaer.de/SCX/DatProt.html for
    // info about SCX protocol

    if ((*pData) != SCX_PROTO_START_HEADER)
    {
        // this message contains an invalid SCX proto message
        m_statCounters.Increment(eStatEntry_MsgBadHeader, 1);
        return;
    }
    pData++;

    // message type
    switch (*pData)
    {
    case e_SCXMsgTypeController:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeController, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastControllerMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastControllerMsg = a_msg;
            ProcessMsgController(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeControllerDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeId:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeId, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastIdMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastIdMsg = a_msg;
            ProcessMsgId(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeIdDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeBusClearance:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeBusClearance, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastBusClearanceMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastBusClearanceMsg = a_msg;
            ProcessMsgBusClearance(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeBusClearanceDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeFinishLine:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeFinishLine, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastFinishLineMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastFinishLineMsg = a_msg;

            // per new Finish line message received at least one
            // lap time message must be processed. Force it here
            // clearing pointer to the last lap time message received
            m_lastLapTimeMsg.clear();

            ProcessMsgFinishLine(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeFinishLineDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeRanking:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeRanking, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastRankingMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastRankingMsg = a_msg;
            ProcessMsgRanking(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeRankingDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeLapTime:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeLapTime, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastLapTimeMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastLapTimeMsg = a_msg;
            ProcessMsgLapTime(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeLapTimeDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeLapCounter:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeLapCounter, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastLapCounterMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastLapCounterMsg = a_msg;
            ProcessMsgLapCounter(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeLapCounterDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeQualifying:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeQualifying, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastQualifyingMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastQualifyingMsg = a_msg;
            ProcessMsgQualifying(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeQualifyingDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeReset:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeReset, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastResetMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastResetMsg = a_msg;
            ProcessMsgReset(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeResetDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeStart:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeStart, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastStartMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastStartMsg = a_msg;
            ProcessMsgStart(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeStartDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeEnd:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeEnd, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastEndMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastEndMsg = a_msg;
            ProcessMsgEnd(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeEndDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeFuel:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeFuel, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastFuelMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastFuelMsg = a_msg;
            ProcessMsgFuel(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeFuelDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeRefreshDisplay:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeRefreshDisplay, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastRefreshDisplayMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastRefreshDisplayMsg = a_msg;
            ProcessMsgRefreshDisplay(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeRefreshDisplayDup, 1);
        }

        break;
    }

    case e_SCXMsgTypeBrake:
    {
        m_statCounters.Increment(eStatEntry_MsgTypeBrake, 1);

        if (!IsDuplicatedMsg(a_msg, m_lastBrakeMsg))
        {
            // this message is not duplicated. Update latest received msg
            m_lastBrakeMsg = a_msg;
            ProcessMsgBrake(pData, a_msg);
        }
        else
        {
            m_statCounters.Increment(eStatEntry_MsgTypeBrakeDup, 1);
        }

        break;
    }


    default:
        // unknown message. No further processing can be done
        qDebug("Unknown message type: %02X", *pData);

        m_statCounters.Increment(eStatEntry_MsgBadType, 1);
        return;
    } // switch (*pData)

}

bool SCXProtoAnalyzer::IsDuplicatedMsg(
        QSharedPointer<QSlotRacingMsg> &a_msgNew,
        QSharedPointer<QSlotRacingMsg> &a_msgOld)
{
    Q_ASSERT(a_msgNew.isNull() == false);

    if (a_msgOld.isNull())
    {
        // latest message contains nothing. Can't be a duplicate
        return false;
    }

    int msecDiff =
            a_msgOld->GetTimestamp().msecsTo(a_msgNew->GetTimestamp());

    Q_ASSERT(msecDiff >= 0);

    if (msecDiff > DUPLICATE_TIMEOUT_MSEC)
    {
        // these two messages are too many milliseconds apart
        // they can't be duplicated
        return false;
    }

    Q_ASSERT(a_msgOld->GetMsgSize() == a_msgNew->GetMsgSize());

    const quint8* pDataOld = a_msgOld->GetBuffer();
    const quint8* pDataNew = a_msgNew->GetBuffer();
    for (qint32 i = 0; i < a_msgOld->GetMsgSize(); i++)
    {
        if ((*pDataOld) != (*pDataNew))
        {
            // can't be duplicated. They don't contain the same data
            return false;
        }

        pDataOld++;
        pDataNew++;
    }

    // All previous checks passed through. Must be a ducplicated message
    return true;
}

void SCXProtoAnalyzer::ProcessMsgController(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventController> event(
            new QSlotRacingEventController(a_msg->GetTimestamp()));

    // The following 6 bytes contain the 6 controller values according to
    // this bit specification:
    // Bits 7, 6: always set to '1'
    // Bit 5: '0' lights ON - '1' lights OFF
    // Bit 4: '0' change button pressed - '1' change button not pressed
    // Bit 3..0: speed value.

    // a_pData now contains C0
    event->AddControllerData(e_QSlotRacingPlayer1, static_cast<quint8>(*a_pData));
    // a_pData now contains C1
    a_pData ++;
    event->AddControllerData(e_QSlotRacingPlayer2, static_cast<quint8>(*a_pData));
    // a_pData now contains C2
    a_pData ++;
    event->AddControllerData(e_QSlotRacingPlayer3, static_cast<quint8>(*a_pData));
    // a_pData now contains C3
    a_pData ++;
    event->AddControllerData(e_QSlotRacingPlayer4, static_cast<quint8>(*a_pData));
    // a_pData now contains C4
    a_pData ++;
    event->AddControllerData(e_QSlotRacingPlayer5, static_cast<quint8>(*a_pData));
    // a_pData now contains C5
    a_pData ++;
    event->AddControllerData(e_QSlotRacingPlayer6, static_cast<quint8>(*a_pData));

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgId(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;
}

void SCXProtoAnalyzer::ProcessMsgBusClearance(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;
}

void SCXProtoAnalyzer::ProcessMsgFinishLine(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    quint8 Car1Byte;
    quint8 Car2Byte;
    quint8 Car3Byte;
    quint8 Car4Byte;
    quint8 Car5Byte;
    quint8 Car6Byte;

    // move past the message type
    a_pData ++;

    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventFinishLine> event(
            new QSlotRacingEventFinishLine(a_msg->GetTimestamp()));

    // The following 6 bytes indicates current finish line crossings
    // Byte 0: car 1 data
    // Byte 1: car 2 data
    // Byte 2: car 3 data
    // Byte 3: car 4 data
    // Byte 4: car 5 data
    // Byte 5: car 6 data
    // If data == 0xE7, the car has crossed the finish line

    // Get car bytes
    Car1Byte = *a_pData;

    // Next car
    a_pData++;
    Car2Byte = *a_pData;

    // Next car
    a_pData++;
    Car3Byte = *a_pData;

    // Next car
    a_pData++;
    Car4Byte = *a_pData;

    // Next car
    a_pData++;
    Car5Byte = *a_pData;

    // Next car
    a_pData++;
    Car6Byte = *a_pData;

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgRanking(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventRanking> event(
            new QSlotRacingEventRanking(a_msg->GetTimestamp()));

    // The following 6 bytes indicates current ranking, depending on byte position
    // Byte 0: position 1 data
    // Byte 1: position 2 data
    // Byte 2: position 3 data
    // Byte 3: position 4 data
    // Byte 4: position 5 data
    // Byte 5: position 6 data
    // Each byte bit format is as follows:
    // Bit 7: '1' unless more that 15 laps behind leader
    // Bit 6..3: laps behind leader
    // bit 2..0: car id

    // a_pData now contains P1
    event->AddRankingData(1, static_cast<quint8>(*a_pData));
    // a_pData now contains P2
    a_pData ++;
    event->AddRankingData(2, static_cast<quint8>(*a_pData));
    // a_pData now contains P3
    a_pData ++;
    event->AddRankingData(3, static_cast<quint8>(*a_pData));
    // a_pData now contains P4
    a_pData ++;
    event->AddRankingData(4, static_cast<quint8>(*a_pData));
    // a_pData now contains P5
    a_pData ++;
    event->AddRankingData(5, static_cast<quint8>(*a_pData));
    // a_pData now contains P6
    a_pData ++;
    event->AddRankingData(6, static_cast<quint8>(*a_pData));

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgLapTime(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{    
    // move past the message type
    a_pData ++;

    // Car id (0-5)
    QSlotRacingPlayer_t currentCar;
    switch(*a_pData)
    {
    case 0:
        currentCar = e_QSlotRacingPlayer1;
        break;
    case 1:
        currentCar = e_QSlotRacingPlayer2;
        break;
    case 2:
        currentCar = e_QSlotRacingPlayer3;
        break;
    case 3:
        currentCar = e_QSlotRacingPlayer4;
        break;
    case 4:
        currentCar = e_QSlotRacingPlayer5;
        break;
    case 5:
        currentCar = e_QSlotRacingPlayer6;
        break;
    default:
        currentCar = e_QSlotRacingNoPlayer;
    }

    // move now past the car id
    a_pData ++;

    // RU1 RU0: Those bytes plus bit 0 from VS result in the number of crossings
    //          Crossings = 256 * RU1 + RU0 + Bit 0 of VS
    //          (VS is the next byte)
    qint32 crossings = (256*(*a_pData)) + *(a_pData + 1) + ( (*(a_pData + 2)) & 0x01);

    // move now past RU1, RU0
    a_pData += 2;

    // lap time. a_pData is pointg now to byte 'VS'
    // Z1 Z0: basevalue = 256 * Z1 + Z0
    // VS:    when bit 3 of the VS byte is set you have to add 256 to the time value
    qint32 basevalue = 256 * (*(a_pData + 1)) + (*(a_pData + 2));
    if ((*a_pData & 0x08) == 0x08)
    {
        // third bit of VS is set
        basevalue += 256;
    }
    // The lap time is basevalue multiplied by 0.01024
    // events are sent in milliseconds (not in seconds) so we should
    // multiply by 10.24 (or multiply by 1024 and then divide by 10
    // to avoid floating point calculations)
    qint32 millis = (basevalue * 1024) / 100;

    QSharedPointer<QSlotRacingEventLap> event(
            new QSlotRacingEventLap(a_msg->GetTimestamp()));

    event->AddLapData(currentCar, crossings, millis);

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgLapCounter(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    quint8 direction;
    quint8 byte0;
    quint8 byte1;
    quint8 byte2;

    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventLapCounter> event(
            new QSlotRacingEventLapCounter(a_msg->GetTimestamp()));

    // move past the message type
    a_pData ++;

    // Get counting direction
    direction = static_cast<quint8>(*a_pData);

    // Get laps data bytes
    a_pData ++;
    byte2 = static_cast<quint8>(*a_pData);
    a_pData ++;
    byte1 = static_cast<quint8>(*a_pData);
    a_pData ++;
    byte0 = static_cast<quint8>(*a_pData);

    // Add event data
    event->AddLapCounterData(direction, byte2, byte1, byte0);

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgQualifying(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    quint8  byte0;
    quint8  byte1;
    quint8  byte2;
    quint16 num_laps;

    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventQualifying> event(
            new QSlotRacingEventQualifying(a_msg->GetTimestamp()));

    // move past the message type
    a_pData ++;

    // Get laps data bytes
    byte2 = (static_cast<quint8>(*a_pData)) & 0x0F;
    a_pData ++;
    byte1 = (static_cast<quint8>(*a_pData)) & 0x0F;
    a_pData ++;
    byte0 = (static_cast<quint8>(*a_pData)) & 0x0F;

    // Calculate number of qualifying laps
    num_laps = (byte2 * 256) + (byte1 * 16) + byte0;

    // Set number of laps
    event->SetNumberOfLaps(num_laps);

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgReset(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;
}

void SCXProtoAnalyzer::ProcessMsgStart(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventStart> event(
            new QSlotRacingEventStart(a_msg->GetTimestamp()));

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgEnd(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventEnd> event(
            new QSlotRacingEventEnd(a_msg->GetTimestamp()));

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgFuel(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{    
    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventFuel> event(
            new QSlotRacingEventFuel(a_msg->GetTimestamp()));

    // move past the message type
    a_pData ++;

    // The following 3 bytes bytes are divided in two nibbles (4 bit long data)
    // The more significant nibble holds the fuel level of the car with the lower id
    // these nibbles of data contain a value from 0 to 8

    // a_pData now contains S0S1
    event->AddFuelData(e_QSlotRacingPlayer1,
                       static_cast<quint8>((*a_pData) & 0xf0) >> 4);
    event->AddFuelData(e_QSlotRacingPlayer2,
                       static_cast<quint8>((*a_pData) & 0x0f));
    a_pData ++;

    // a_pData now contains S2S3
    event->AddFuelData(e_QSlotRacingPlayer3,
                       static_cast<quint8>((*a_pData) & 0xf0) >> 4);
    event->AddFuelData(e_QSlotRacingPlayer4,
                       static_cast<quint8>((*a_pData) & 0x0f));
    a_pData ++;

    // a_pData now contains S4S5
    event->AddFuelData(e_QSlotRacingPlayer5,
                       static_cast<quint8>((*a_pData) & 0xf0) >> 4);
    event->AddFuelData(e_QSlotRacingPlayer6,
                       static_cast<quint8>((*a_pData) & 0x0f));

    // notify the event to upper layers (whoever that might be)
    emit ProtoEvent(event);
}

void SCXProtoAnalyzer::ProcessMsgRefreshDisplay(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;
}

void SCXProtoAnalyzer::ProcessMsgBrake(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;
}
