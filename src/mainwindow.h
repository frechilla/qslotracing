#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include "qslotracingevent.h"
#include "snifferserial.h"
#include "serialmonitor.h"
#include "configdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    // from QT documentation:
    //  simply tells the pre-compiler that this class has gui elements and
    //  needs to be run through the 'moc' you only need to add this to classes
    //  that use the signal/slot mechanism
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /// @brief entry point for events
    /// events will be processed by the window and the GUI will be
    /// updated accordingly
    /// @param shared pointer wrapping the event
    void ProcessEvent(QSharedPointer<QSlotRacingEvent> a_event);

    /// @brief initializes fuel devices background color
    /// @param none
    void InitFuelBackground(void);

    /// @brief initializes fuel devices background color
    /// @param none
    void SetCar1Fuel(quint8 value);
    void SetCar2Fuel(quint8 value);
    void SetCar3Fuel(quint8 value);
    void SetCar4Fuel(quint8 value);
    void SetCar5Fuel(quint8 value);
    void SetCar6Fuel(quint8 value);

private:
    Ui::MainWindow *ui;
    SnifferSerial m_serial;
    SerialMonitor m_monitor;
    ConfigDialog m_config;
    void OpenSerialPort(void);

    void ConfigurePlayer1(QString player, bool flag, int car);
    void ConfigurePlayer2(QString player, bool flag, int car);
    void ConfigurePlayer3(QString player, bool flag, int car);
    void ConfigurePlayer4(QString player, bool flag, int car);
    void ConfigurePlayer5(QString player, bool flag, int car);
    void ConfigurePlayer6(QString player, bool flag, int car);

private slots:
    void on_serial_monitor_clicked();
    void slotRead(const quint8* a_buffer, quint32 a_bufferSize);
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_BtnConfigure_clicked();

    void SetMainWindowDelegate();
};

#endif // MAINWINDOW_H
