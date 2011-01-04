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
}

void SCXProtoAnalyzer::ProcessMsgLapTime(
        const quint8* a_pData,
        const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    // move past the message type
    a_pData ++;

    std::cout << "LapTime message"
              << std::endl;
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
                       static_cast<quint8>((*a_pData) & 0xf0));
    event->AddFuelData(e_QSlotRacingPlayer2,
                       static_cast<quint8>((*a_pData) & 0x0f));
    a_pData ++;

    // a_pData now contains S2S3
    event->AddFuelData(e_QSlotRacingPlayer3,
                       static_cast<quint8>((*a_pData) & 0xf0));
    event->AddFuelData(e_QSlotRacingPlayer4,
                       static_cast<quint8>((*a_pData) & 0x0f));
    a_pData ++;

    // a_pData now contains S4S5
    event->AddFuelData(e_QSlotRacingPlayer5,
                       static_cast<quint8>((*a_pData) & 0xf0));
    event->AddFuelData(e_QSlotRacingPlayer6,
                       static_cast<quint8>((*a_pData) & 0x0f));

    // notify the event to upper layers (whoever that might be)
    m_eventDelegate(event);

    std::cout << "Fuel message"
              << std::endl;
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
