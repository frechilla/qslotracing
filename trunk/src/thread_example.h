#ifndef THREAD_EXAMPLE_H
#define THREAD_EXAMPLE_H
#include <QtCore>
#include <QtCore/QDebug>

#include <qthread.h>

class GenEventos : public QObject
{
    Q_OBJECT
    QByteArray data;

public:
    inline GenEventos(){qDebug()<<"crear..."; }

public slots:
    void gen_event()
    {
        qDebug()<<"producing...";
        emit produced(&data);
    }

signals:
    void produced(QByteArray *data);
    void finished();
};


class Thread : public QThread
{
    Q_OBJECT

public:
    Thread();
    void stop();

protected:
    void run();

private:
};



#endif // THREAD_EXAMPLE_H
