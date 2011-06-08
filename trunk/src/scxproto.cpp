#include "scxproto.h"
#include <iostream>

// first byte MUST always be 0x55
#define SCX_PROTO_START_HEADER 0x55

SCXProtoAnalyzer::SCXProtoAnalyzer() :
        m_eventDelegate() // null delegate for now
{
}

SCXProtoAnalyzer::~SCXProtoAnalyzer()
{
}

void SCXProtoAnalyzer::SetEventProcessorDelegate(
        QSlotRacingEventDelegate_t a_eventDelegate)
{
    m_eventDelegate = a_eventDelegate;
}

void SCXProtoAnalyzer::ProcessMsg(QSharedPointer<QSlotRacingMsg> a_msg)
{
    //TODO remove
    for (qint32 i = 0; i < a_msg->GetMsgSize(); i++)
    {
        std::cout << std::hex
                  << static_cast<quint32>(a_msg->GetMsg()[i])
                  << " ";

    }
    std::cout << std::endl;

    const quint8* pData = a_msg->GetBuffer();

    // see http://eng.slotbaer.de/SCX/DatProt.html for
    // info about SCX protocol

    if ((*pData) != SCX_PROTO_START_HEADER)
    {
        // this message contains an invalid SCX proto message
        return;
    }
    pData++;

    // message type
    switch (*pData)
    {
    case e_SCXMsgTypeController:
    {
        ProcessMsgController(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeId:
    {
        ProcessMsgId(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeBusClearance:
    {
        ProcessMsgBusClearance(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeFinishLine:
    {
        ProcessMsgFinishLine(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeRanking:
    {
        ProcessMsgRanking(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeLapTime:
    {
        ProcessMsgLapTime(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeLapCounter:
    {
        ProcessMsgLapCounter(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeQualifying:
    {
        ProcessMsgQualifying(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeReset:
    {
        ProcessMsgReset(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeStart:
    {
        ProcessMsgStart(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeEnd:
    {
        ProcessMsgEnd(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeFuel:
    {
        ProcessMsgFuel(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeRefreshDisplay:
    {
        ProcessMsgRefreshDisplay(pData, a_msg);
        break;
    }

    case e_SCXMsgTypeBrake:
    {
        ProcessMsgBrake(pData, a_msg);
        break;
    }


    default:
        // unknown message. No further processing can be done
        std::cout << "Unknown message type: "
                  << std::hex << static_cast<quint32>(*pData)
                  << std::endl;
        return;
    } // switch (*pData)

}

void SCXProtoAnalyzer::ProcessMsgController(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Controller message"
              << std::endl;

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
    m_eventDelegate(event);
}

void SCXProtoAnalyzer::ProcessMsgId(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Id message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgBusClearance(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "BusClearance message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgFinishLine(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "FinishLine message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgRanking(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Ranking message"
              << std::endl;

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
    event->AddRankingData(e_QSlotRacingPlayer1, static_cast<quint8>(*a_pData));
    // a_pData now contains P2
    a_pData ++;
    event->AddRankingData(e_QSlotRacingPlayer2, static_cast<quint8>(*a_pData));
    // a_pData now contains P3
    a_pData ++;
    event->AddRankingData(e_QSlotRacingPlayer3, static_cast<quint8>(*a_pData));
    // a_pData now contains P4
    a_pData ++;
    event->AddRankingData(e_QSlotRacingPlayer4, static_cast<quint8>(*a_pData));
    // a_pData now contains P5
    a_pData ++;
    event->AddRankingData(e_QSlotRacingPlayer5, static_cast<quint8>(*a_pData));
    // a_pData now contains P6
    a_pData ++;
    event->AddRankingData(e_QSlotRacingPlayer6, static_cast<quint8>(*a_pData));

    // notify the event to upper layers (whoever that might be)
    m_eventDelegate(event);

}

void SCXProtoAnalyzer::ProcessMsgLapTime(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{    
    std::cout << "LapTime message"
              << std::endl;

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


    // this is the event that will be pass through to upper layers
    QSharedPointer<QSlotRacingEventLap> event(
            new QSlotRacingEventLap(a_msg->GetTimestamp(),
                                    currentCar,
                                    crossings,
                                    millis));

    // notify the event to upper layers (whoever that might be)
    m_eventDelegate(event);
}

void SCXProtoAnalyzer::ProcessMsgLapCounter(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "LapCounter message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgQualifying(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Qualifying message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgReset(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Reset message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgStart(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Start message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgEnd(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "End message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgFuel(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{    
    std::cout << "Fuel message"
              << std::endl;

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
    m_eventDelegate(event);
}

void SCXProtoAnalyzer::ProcessMsgRefreshDisplay(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "RefreshDisplay message"
              << std::endl;
}

void SCXProtoAnalyzer::ProcessMsgBrake(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "Brake message"
              << std::endl;
}
