#include "scxproto.h"
#include <iostream>

// first byte MUST always be 0x55
#define SCX_PROTO_START_HEADER 0x55

void scxproto::ProcessMsg(QSharedPointer<QSlotRacingMsg> a_msg)
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
        ProcessMsgController(a_msg);
        break;
    }

    case e_SCXMsgTypeId:
    {
        ProcessMsgId(a_msg);
        break;
    }

    case e_SCXMsgTypeBusClearance:
    {
        ProcessMsgBusClearance(a_msg);
        break;
    }

    case e_SCXMsgTypeFinishLine:
    {
        ProcessMsgFinishLine(a_msg);
        break;
    }

    case e_SCXMsgTypeRanking:
    {
        ProcessMsgRanking(a_msg);
        break;
    }

    case e_SCXMsgTypeLapTime:
    {
        ProcessMsgLapTime(a_msg);
        break;
    }

    case e_SCXMsgTypeLapCounter:
    {
        ProcessMsgLapCounter(a_msg);
        break;
    }

    case e_SCXMsgTypeQualifying:
    {
        ProcessMsgQualifying(a_msg);
        break;
    }

    case e_SCXMsgTypeReset:
    {
        ProcessMsgReset(a_msg);
        break;
    }

    case e_SCXMsgTypeStart:
    {
        ProcessMsgStart(a_msg);
        break;
    }

    case e_SCXMsgTypeEnd:
    {
        ProcessMsgEnd(a_msg);
        break;
    }

    case e_SCXMsgTypeFuel:
    {
        ProcessMsgFuel(a_msg);
        break;
    }

    case e_SCXMsgTypeRefreshDisplay:
    {
        ProcessMsgRefreshDisplay(a_msg);
        break;
    }

    case e_SCXMsgTypeBrake:
    {
        ProcessMsgBrake(a_msg);
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

void scxproto::ProcessMsgController(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Controller message"
              << std::endl;
}

void scxproto::ProcessMsgId(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Id message"
              << std::endl;
}

void scxproto::ProcessMsgBusClearance(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "BusClearance message"
              << std::endl;
}

void scxproto::ProcessMsgFinishLine(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "FinishLine message"
              << std::endl;
}

void scxproto::ProcessMsgRanking(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Ranking message"
              << std::endl;
}

void scxproto::ProcessMsgLapTime(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "LapTime message"
              << std::endl;
}

void scxproto::ProcessMsgLapCounter(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "LapCounter message"
              << std::endl;
}

void scxproto::ProcessMsgQualifying(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Qualifying message"
              << std::endl;
}

void scxproto::ProcessMsgReset(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Reset message"
              << std::endl;
}

void scxproto::ProcessMsgStart(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Start message"
              << std::endl;
}

void scxproto::ProcessMsgEnd(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "End message"
              << std::endl;
}

void scxproto::ProcessMsgFuel(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Fuel message"
              << std::endl;
}

void scxproto::ProcessMsgRefreshDisplay(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "RefreshDisplay message"
              << std::endl;
}

void scxproto::ProcessMsgBrake(const QSharedPointer<QSlotRacingMsg> &a_msg)
{
    const quint8* pData = a_msg->GetBuffer();

    // someone must have checked the header and
    // message type already.
    // go past the HEADER and the message type
    pData += 2;

    std::cout << "Brake message"
              << std::endl;
}
