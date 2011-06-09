#include <QtCore/QDebug>
#include "thread_example.h"

Thread::Thread()
{
}

void Thread::run()
{
    qDebug()<<"running thread...";
    exec();
}

void Thread::stop()
{
}
