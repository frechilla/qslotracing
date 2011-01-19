#ifndef ABSTRACTSERIALENGINE_P_H
#define ABSTRACTSERIALENGINE_P_H

#include "abstractserialengine.h"


class AbstractSerialEnginePrivate
{
public:
    AbstractSerialEnginePrivate();

    static const char defaultDeviceName[];

    QString deviceName;
    AbstractSerial::BaudRate ibaudRate;
    AbstractSerial::BaudRate obaudRate;
    AbstractSerial::DataBits dataBits;
    AbstractSerial::Parity parity;
    AbstractSerial::StopBits stopBits;
    AbstractSerial::Flow flow;
    int charIntervalTimeout;
    AbstractSerial::Status status;
    bool oldSettingsIsSaved;
    //
    Serial::DESCRIPTOR descriptor;
    AbstractSerialEngineReceiver *receiver;
    bool isAutoCalcReadTimeoutConstant;

    quint32 currInBaudRateToValue() const;
    int currOutBaudRateToValue() const;
    int currDataBitsToValue() const;
    int currParityToValue() const;
    int currStopBitsToValue() const;

};

#endif // ABSTRACTSERIALENGINE_P_H
