#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QDialog>
#include <QFile>
#include <QDataStream>

namespace Ui {
    class SerialMonitor;
}

class SerialMonitor : public QDialog
{
    Q_OBJECT

public:
    explicit SerialMonitor(QWidget *parent = 0);
    ~SerialMonitor();

    void AddTrace();

private:
    Ui::SerialMonitor *ui;
    int flag_show;
    QFile* m_file;

private slots:
    void on_save_clicked();
    void on_stop_capture_clicked();
    void on_start_capture_clicked();
};

#endif // SERIALMONITOR_H
