#ifndef QSLOTRACINGMSGFACTORY_H
#define QSLOTRACINGMSGFACTORY_H

#include <QObject>
#include <QByteArray>
#include <QSharedPointer> // wrap QSlotRacingMsg objects and share them accross
                          // several threads
#include "qslotracingmsg.h"

/// @brief vitual class to be inherited by all factories of QSlotRacingMsgs
/// Children classes whould try to separate the flow of input data into separate
/// protocol units (QSlotRacinsgMsg) based on different digital slot car racing
/// protocols
class QSlotRacingMsgFactory : public QObject
{
    Q_OBJECT

public:
    QSlotRacingMsgFactory(QObject *parent = 0) :
            QObject(parent)
    {}
    virtual ~QSlotRacingMsgFactory()
    {}


signals:
    /// @brief signal sent when a brand new QSlotRacingMsg is parsed
    void MsgParsed(QSharedPointer<QSlotRacingMsg>);

    /// @brief signal sent when minimun number of valid messages read
    void ProtocolSynced(bool);

public slots:
    /// @brief parses a buffer of bytes of size 'a_bufferSize'
    /// its output is one or more new QSlotRacingMsg object(s)
    /// @param a_dataBuffer byte buffer to be parsed
    virtual void Parse(QByteArray a_dataBuffer) = 0;
};

#endif // QSLOTRACINGMSGFACTORY_H
