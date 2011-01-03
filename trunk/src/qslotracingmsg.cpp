#include <QtGlobal> // Q_ASSERT
#include "qslotracingmsg.h"

QSlotRacingMsg::QSlotRacingMsg(quint32 a_msgSize, quint8* a_msgBuffer) :
        m_data(a_msgSize),
        m_timestamp(QTime::currentTime())
{
    for (quint32 i = 0; i < a_msgSize; i++)
    {
        m_data[i] = a_msgBuffer[i];
    }
}

QSlotRacingMsg::~QSlotRacingMsg()
{
}

void QSlotRacingMsg::SetAt(quint8 a_newByte, quint32 a_index)
{
    // useful for testing pre- and post-conditions during development.
    // It does nothing if QT_NO_DEBUG was defined during compilation
    Q_ASSERT((a_index >= 0) &&
             (a_index < static_cast<quint32>(m_data.size())));

    m_data[a_index] = a_newByte;
}
