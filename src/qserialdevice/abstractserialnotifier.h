#ifndef ABSTRACTSERIALNOTIFIER_H
#define ABSTRACTSERIALNOTIFIER_H

#include "nativeserialengine.h"


class AbstractSerialNotifier
{
public:
    static AbstractSerialNotifier *createSerialNotifier(NativeSerialEngine *parent);
    void setDescriptor(Serial::DESCRIPTOR descriptor);

    virtual bool isReadNotificationEnabled() const = 0;
    virtual void setReadNotificationEnabled(bool enable) = 0;
    virtual bool isWriteNotificationEnabled() const = 0;
    virtual void setWriteNotificationEnabled(bool enable) = 0;
    virtual bool isExceptionNotificationEnabled() const = 0;
    virtual void setExceptionNotificationEnabled(bool enable) = 0;
    virtual bool isLineNotificationEnabled() const = 0;
    virtual void setLineNotificationEnabled(bool enable) = 0;

protected:
    Serial::DESCRIPTOR descriptor;
    NativeSerialEngine *engine;
};


#endif // ABSTRACTSERIALNOTIFIER_H
