#ifndef THREAD_EXAMPLE_H
#define THREAD_EXAMPLE_H
#include <QtCore>
#include <QtCore/QDebug>
#include <qthread.h>

#include "snifferserial.h"


class SerialEvent : public QObject
{
    Q_OBJECT
    QByteArray data;

public:
    inline SerialEvent(){qDebug()<<"Creating SerialEvent thread..."; }

    void SetSerialDevice(SnifferSerial* device)
    {
        m_pSnifferSerial = device;
        qDebug()<<"Assingned serial device...";
    }

private:
    SnifferSerial* m_pSnifferSerial;

public slots:
    void gen_event()
    {
        while(1)
        {
            m_pSnifferSerial->Read(10, data);
            emit DataRead(&data);
        }
    }

signals:
    void DataRead(QByteArray *data);
    void finished();
};


class SerialThread : public QThread
{
    Q_OBJECT

public:
    SerialThread();
    void stop();

protected:
    void run();

private:
};



#endif // SERIALTHREAD_H
