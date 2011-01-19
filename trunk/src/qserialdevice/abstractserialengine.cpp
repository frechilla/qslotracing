/*
* This file is part of QSerialDevice, an open-source cross-platform library
* Copyright (C) 2009  Denis Shienkov
*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact Denis Shienkov:
*          e-mail: <scapig2@yandex.ru>
*             ICQ: 321789831
*/


#include "nativeserialengine.h"
#include "abstractserialengine_p.h"


AbstractSerialEnginePrivate::AbstractSerialEnginePrivate()
        : deviceName(QString())
        , ibaudRate(AbstractSerial::BaudRate9600)
        , obaudRate(AbstractSerial::BaudRate9600)
        , dataBits(AbstractSerial::DataBits8)
        , parity(AbstractSerial::ParityNone)
        , stopBits(AbstractSerial::StopBits1)
        , flow(AbstractSerial::FlowControlOff)
        , charIntervalTimeout(0)
        , status(AbstractSerial::ENone)
        , oldSettingsIsSaved(false)
        , descriptor(0)
        , receiver(0)
        , isAutoCalcReadTimeoutConstant(false)
{
}

#ifdef Q_OS_WIN32
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "COM1";
#elif defined(Q_OS_IRIX)
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "/dev/ttyf1";
#elif defined(Q_OS_HPUX)
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "/dev/tty1p0";
#elif defined(Q_OS_SOLARIS)
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "/dev/ttya";
#elif defined(Q_OS_FREEBSD)
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "/dev/ttyd1";
#elif defined(Q_OS_LINUX)
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "/dev/ttyS0";
#else
    const char AbstractSerialEnginePrivate::defaultDeviceName[] = "/dev/ttyS0";
#endif


static quint32 baud_to_value(AbstractSerial::BaudRate param)
{
    switch (param) {
    case AbstractSerial::BaudRate50: return 50;
    case AbstractSerial::BaudRate75: return 75;
    case AbstractSerial::BaudRate110: return 110;
    case AbstractSerial::BaudRate134: return 134;
    case AbstractSerial::BaudRate150: return 150;
    case AbstractSerial::BaudRate200: return 200;
    case AbstractSerial::BaudRate300: return 300;
    case AbstractSerial::BaudRate600: return 600;
    case AbstractSerial::BaudRate1200: return 1200;
    case AbstractSerial::BaudRate1800: return 1800;
    case AbstractSerial::BaudRate2400: return 2400;
    case AbstractSerial::BaudRate4800: return 4800;
    case AbstractSerial::BaudRate9600: return 9600;
    case AbstractSerial::BaudRate14400: return 14400;
    case AbstractSerial::BaudRate19200: return 19200;
    case AbstractSerial::BaudRate38400: return 38400;
    case AbstractSerial::BaudRate56000: return 56000;
    case AbstractSerial::BaudRate57600: return 57600;
    case AbstractSerial::BaudRate76800: return 76800;
    case AbstractSerial::BaudRate115200: return 115200;
    case AbstractSerial::BaudRate128000: return 128000;
    case AbstractSerial::BaudRate230400: return 230400;
    case AbstractSerial::BaudRate256000: return 256000;
    case AbstractSerial::BaudRate460800: return 460800;
    case AbstractSerial::BaudRate500000: return 500000;
    case AbstractSerial::BaudRate576000: return 576000;
    case AbstractSerial::BaudRate921600: return 921600;
    case AbstractSerial::BaudRate1000000: return 1000000;
    case AbstractSerial::BaudRate1152000: return 1152000;
    case AbstractSerial::BaudRate1500000: return 1500000;
    case AbstractSerial::BaudRate2000000: return 2000000;
    case AbstractSerial::BaudRate2500000: return 2500000;
    case AbstractSerial::BaudRate3000000: return 3000000;
    case AbstractSerial::BaudRate3500000: return 3500000;
    case AbstractSerial::BaudRate4000000: return 4000000;
    default:;
    }
    return 0;
}

quint32 AbstractSerialEnginePrivate::currInBaudRateToValue() const
{
    return baud_to_value(this->ibaudRate);
}

int AbstractSerialEnginePrivate::currOutBaudRateToValue() const
{
    return baud_to_value(this->obaudRate);
}

int AbstractSerialEnginePrivate::currDataBitsToValue() const
{
    switch (this->dataBits) {
    case AbstractSerial::DataBits5: return 5;
    case AbstractSerial::DataBits6: return 6;
    case AbstractSerial::DataBits7: return 7;
    case AbstractSerial::DataBits8: return 8;
    default:;
    }
    return 8;
}

int AbstractSerialEnginePrivate::currParityToValue() const
{
    switch (this->parity) {
    case AbstractSerial::ParityNone: return 0;
    default:;
    }
    return 1;
}

int AbstractSerialEnginePrivate::currStopBitsToValue() const
{
    switch (this->stopBits) {
    case AbstractSerial::StopBits2: return 2;
    default:;
    }
    return 1;
}

//---------------------------------------------------------------------------//

AbstractSerialEngine::AbstractSerialEngine(QObject *parent)
    : QObject(parent)
    , d_ptr(new AbstractSerialEnginePrivate())
{
}

AbstractSerialEngine::AbstractSerialEngine(AbstractSerialEnginePrivate &dd, QObject *parent)
    : QObject(parent)
    , d_ptr(&dd)
{
}

AbstractSerialEngine::~AbstractSerialEngine()
{
    delete d_ptr;
}

AbstractSerialEngine *AbstractSerialEngine::createSerialEngine(QObject *parent)
{
    return new NativeSerialEngine(parent);
}

void AbstractSerialEngine::setDeviceName(const QString &deviceName)
{
    d_func()->deviceName = deviceName;
}

QString AbstractSerialEngine::deviceName() const
{
    return d_func()->deviceName;
}

AbstractSerial::BaudRate AbstractSerialEngine::baudRate() const
{
    return (d_func()->ibaudRate == d_func()->obaudRate) ?
            d_func()->ibaudRate : AbstractSerial::BaudRateUndefined;
}

AbstractSerial::BaudRate AbstractSerialEngine::inputBaudRate() const
{
    return d_func()->ibaudRate;
}

AbstractSerial::BaudRate AbstractSerialEngine::outputBaudRate() const
{
    return d_func()->obaudRate;
}

AbstractSerial::DataBits AbstractSerialEngine::dataBits() const
{
    return d_func()->dataBits;
}

AbstractSerial::Parity AbstractSerialEngine::parity() const
{
    return d_func()->parity;
}

AbstractSerial::StopBits AbstractSerialEngine::stopBits() const
{
    return d_func()->stopBits;
}

AbstractSerial::Flow AbstractSerialEngine::flow() const
{
    return d_func()->flow;
}

AbstractSerial::Status AbstractSerialEngine::status() const
{
    return d_func()->status;
}

void AbstractSerialEngine::setReceiver(AbstractSerialEngineReceiver *receiver)
{
    d_func()->receiver = receiver;
}

void AbstractSerialEngine::readNotification()
{
    if (AbstractSerialEngineReceiver *receiver = d_func()->receiver)
        receiver->readNotification();
}

void AbstractSerialEngine::writeNotification()
{
    if (AbstractSerialEngineReceiver *receiver = d_func()->receiver)
        receiver->writeNotification();
}

void AbstractSerialEngine::exceptionNotification()
{
    if (AbstractSerialEngineReceiver *receiver = d_func()->receiver)
        receiver->exceptionNotification();
}

void AbstractSerialEngine::lineNotification()
{
    if (AbstractSerialEngineReceiver *receiver = d_func()->receiver)
        receiver->lineNotification();
}


#include "moc_abstractserialengine.cpp"
