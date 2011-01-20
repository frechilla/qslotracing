#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <abstractserial.h>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = 0);
    void Write(void);
    void OpenSerial(void);

signals:

public slots:
    void slotRead(void);

private:
    AbstractSerial* m_port;
};

#endif // SERIAL_H
