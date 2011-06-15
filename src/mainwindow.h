#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include "qslotracingevent.h"
#include "snifferserial.h"
#include "serialmonitor.h"
#include "configdialog.h"
#include "controllerdlg.h"
#include "statsdialog.h"
#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"

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

    void SetController1(bool lights, bool lane_change, quint8 speed);
    void SetController2(bool lights, bool lane_change, quint8 speed);
    void SetController3(bool lights, bool lane_change, quint8 speed);
    void SetController4(bool lights, bool lane_change, quint8 speed);
    void SetController5(bool lights, bool lane_change, quint8 speed);
    void SetController6(bool lights, bool lane_change, quint8 speed);

    /// @brief update race status
    void UpdateRaceStatus(int status);
private:

    typedef QVector<quint32>  PlayersTimeContainerType_t;
    typedef QList<quint32> PlayersTimesType_t;


    // Member variables
    Ui::MainWindow *ui;
    SerialMonitor m_monitor;
    ConfigDialog m_config;
    ControllerDlg m_controller;
    StatsDialog m_statsdlg;

    /// Sniff data off the serial interface
    SnifferSerial m_serialSniffer;

    /// ASCII sniffer (to simulate binary input reading from a file)
    /// (used mainly for debug)
    SnifferFileAscii m_asciiSniffer;

    /// parses the raw data cominf from the sniffers to create QSlotRacingMsg's
    SCXMsgFactory    m_msgFactory;

    /// SCX protocol analyzer. Takes in QSlotRacingMsg's and create QSlotRacingEvent's
    SCXProtoAnalyzer m_scxAnalyzer;

    /// timer to print out stats
    QTimer m_statsTimer;

    PlayersTimeContainerType_t m_PlayersBestTimes;

    PlayersTimesType_t m_Player1RaceTimes;
    PlayersTimesType_t m_Player2RaceTimes;
    PlayersTimesType_t m_Player3RaceTimes;
    PlayersTimesType_t m_Player4RaceTimes;
    PlayersTimesType_t m_Player5RaceTimes;
    PlayersTimesType_t m_Player6RaceTimes;

    quint32 m_BestRaceLapTime;


    // Laps counting direction
    quint8 m_CountingDir;

    // Laps counter
    quint32 m_LapsCounter;

    /// @brief initialize all objects related to sniffing and protocol decoding
    /// Connects also all signals to the expected slots
    /// WARNING: should be ONLY called once
    void InitializeProtoStack();

    void OpenSerialPort(QString port);

    /// @brief configuration procedure for Player 1
    /// @param player name
    /// @param player active flag
    /// @param player car ID
    void ConfigurePlayer1(QString player, bool flag, int car);

    /// @brief configuration procedure for Player 2
    /// @param player name
    /// @param player active flag
    /// @param player car ID
    void ConfigurePlayer2(QString player, bool flag, int car);

    /// @brief configuration procedure for Player 3
    /// @param player name
    /// @param player active flag
    /// @param player car ID
    void ConfigurePlayer3(QString player, bool flag, int car);

    /// @brief configuration procedure for Player 4
    /// @param player name
    /// @param player active flag
    /// @param player car ID
    void ConfigurePlayer4(QString player, bool flag, int car);

    /// @brief configuration procedure for Player 5
    /// @param player name
    /// @param player active flag
    /// @param player car ID
    void ConfigurePlayer5(QString player, bool flag, int car);

    /// @brief configuration procedure for Player 6
    /// @param player name
    /// @param player active flag
    /// @param player car ID
    void ConfigurePlayer6(QString player, bool flag, int car);

    void UpdateCarPosition(quint8 carId, quint8 pos, bool carFlag, bool moreThan15, quint8 lapsBehind);

    /// @brief update laps showed in window
    void UpdateLaps();

    /// @brief initializes status frame withing window
    void InitStatusFrame();

    /// @brief Format a string from a given time in milliseconds
    /// @param Returned formated string
    /// @param Provided time in milliseconds
    void GetStringFromTime(QString &timestr, quint32 time);

    /// @brief Check if provided time is the best partial time for player
    /// @param Current player
    /// @param Provided time
    bool IsBestOwnTime(QSlotRacingPlayer_t player, quint32 time);

    /// @brief Check if provided time is the best partial time for player
    /// @param Current player
    /// @param Provided time
    bool IsFasterLap(QSlotRacingPlayer_t player, quint32 time);

    /// @brief Check if provided time is the best race lap time
    /// @param Provided time
    bool IsBestLapTime(quint32 time);

private slots:

    /// @brief entry point for events
    /// events will be processed by the window and the GUI will be
    /// updated accordingly
    /// @param shared pointer wrapping the event
    void ProcessEvent(QSharedPointer<QSlotRacingEvent> a_event);

    /// @brief update synchro status
    void UpdateSynchroStatus(bool bSynchro);

    /// @brief handler for the stats timer.
    /// Stats will be collected from the modules and printed out
    void UpdateStats();

    void on_BtnConfigure_clicked();
    void on_btnStats_clicked();
    void on_btnController_clicked();
    void on_serial_monitor_clicked();

    //TODO remove
    void slotRead(QByteArray);

    // TODO: remove?
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
public slots:
    void consume(QByteArray *data);
};

#endif // MAINWINDOW_H
