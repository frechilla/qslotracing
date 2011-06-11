#include <QtCore/QDebug>
#include "serialthread.h"

SerialThread::SerialThread()
{
}

void SerialThread::run()
{
    qDebug()<<"running thread...";
    exec();
}

void SerialThread::stop()
{
}
