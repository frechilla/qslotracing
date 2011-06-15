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

    /// @brief Sets current fuel level for car 1. It updates fuel display
    /// for car
    /// @param current fuel level
    void SetCar1Fuel(quint8 value);

    /// @brief Sets current fuel level for car 2. It updates fuel display
    /// for car
    /// @param current fuel level
    void SetCar2Fuel(quint8 value);

    /// @brief Sets current fuel level for car 3. It updates fuel display
    /// for car
    /// @param current fuel level
    void SetCar3Fuel(quint8 value);

    /// @brief Sets current fuel level for car 4. It updates fuel display
    /// for car
    /// @param current fuel level
    void SetCar4Fuel(quint8 value);

    /// @brief Sets current fuel level for car 5. It updates fuel display
    /// for car
    /// @param current fuel level
    void SetCar5Fuel(quint8 value);

    /// @brief Sets current fuel level for car 6. It updates fuel display
    /// for car
    /// @param current fuel level
    void SetCar6Fuel(quint8 value);

    /// @brief Set controller status for controller id 1.
    /// @param ligths flag
    /// @param lane change button flag
    /// @param current speed value
    void SetController1(bool lights, bool lane_change, quint8 speed);

    /// @brief Set controller status for controller id 2.
    /// @param ligths flag
    /// @param lane change button flag
    /// @param current speed value
    void SetController2(bool lights, bool lane_change, quint8 speed);

    /// @brief Set controller status for controller id 3.
    /// @param ligths flag
    /// @param lane change button flag
    /// @param current speed value
    void SetController3(bool lights, bool lane_change, quint8 speed);

    /// @brief Set controller status for controller id 4.
    /// @param ligths flag
    /// @param lane change button flag
    /// @param current speed value
    void SetController4(bool lights, bool lane_change, quint8 speed);

    /// @brief Set controller status for controller id 5.
    /// @param ligths flag
    /// @param lane change button flag
    /// @param current speed value
    void SetController5(bool lights, bool lane_change, quint8 speed);

    /// @brief Set controller status for controller id 6.
    /// @param ligths flag
    /// @param lane change button flag
    /// @param current speed value
    void SetController6(bool lights, bool lane_change, quint8 speed);

    /// @brief update race status
    void UpdateRaceStatus(int status);
private:

    /// Class type definitions
    typedef QVector<quint32>  PlayersTimeContainerType_t;
   // typedef QList<quint32> PlayersTimesType_t;

    /// Class member variables

    /// @brief main window pointer
    Ui::MainWindow *ui;

    /// @brief serial monitor dialog member variable
    SerialMonitor m_monitor;

    /// @brief configuration dialog member variable
    ConfigDialog m_config;

    /// @brief controller dialog member variable
    ControllerDlg m_controller;

    /// @brief stats dialog member variable
    StatsDialog m_statsdlg;

    /// @brief Sniff data off the serial interface
    SnifferSerial m_serialSniffer;

    /// @brief ASCII sniffer (to simulate binary input reading from a file)
    /// (used mainly for debug)
    SnifferFileAscii m_asciiSniffer;

    /// @brief parses the raw data cominf from the sniffers to create QSlotRacingMsg's
    SCXMsgFactory    m_msgFactory;

    /// @brief SCX protocol analyzer. Takes in QSlotRacingMsg's and create QSlotRacingEvent's
    SCXProtoAnalyzer m_scxAnalyzer;

    /// @brief timer to print out stats
    QTimer m_statsTimer;

    /// @brief vector containing players best times
    PlayersTimeContainerType_t m_PlayersBestTimes;

    /// @brief variable containing current best race lap time player
    QSlotRacingPlayer_t m_CurPlayerBestLapTime;

    /// @brief variable containing last player race lap time player
    PlayersTimeContainerType_t m_PlayerLastLapTime;

    /// @brief variable containing player number of crossings
    PlayersTimeContainerType_t m_PlayerCrossings;

    /// @brief Array of configured players flags
    bool m_PlayersConfigured[6];

    /// @brief Race configured counting direction
    quint8 m_CountingDir;

    /// @brief Race configured number of laps
    quint32 m_LapsCounter;

    /// @brief initialize all objects related to sniffing and protocol decoding
    /// Connects also all signals to the expected slots
    /// WARNING: should be ONLY called once
    void InitializeProtoStack();

    /// @brief this method opens and starts the serial communication device
    /// @param serial port string identifier COM1, COM2, ...
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

    /// @brief This method updates car position at main window GUI
    /// @param car identifier
    /// @param current car position
    /// @param defines if the car is present or not
    /// @param more than 15 laps behind leader flag
    /// @param laps begind leader
    void UpdateCarPosition(quint8 carId, quint8 pos, bool carFlag, bool moreThan15, quint8 lapsBehind);

    /// @brief update laps showed in window
    void UpdateLaps();

    /// @brief initializes status frame withing window
    void InitStatusFrame();

    /// @brief Format a string from a given time in milliseconds
    /// @param Returned formated string
    /// @param Provided time in milliseconds
    void GetStringFromTime(QString *timestr, quint32 time);

    /// @brief Check if provided time is the best partial time for player
    /// @param Current player
    /// @param Provided time
    bool IsFasterLap(QSlotRacingPlayer_t player, quint32 time);

    /// @brief Initialize timing strings
    void InitTimingStrings();

    /// @brief This method updates the status of the player best time edit box
    void UpdateRaceBestLapTime(QSlotRacingPlayer_t player, quint32 curtime, quint32 crossing);

    /// @brief This method updates the status of the player current lap time edit box
    void UpdatePlayerLapTime(QSlotRacingPlayer_t player, quint32 curtime, quint32 crossing);

    /// @brief Check if there is a new crossing for the provided player
    /// @param current player lap event
    /// @param number of crossings
    bool IsNewCrossing(QSlotRacingPlayer_t player, quint32 crossing);

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

    /// @brief Configuration dialog click button slot
    void on_BtnConfigure_clicked();

    /// @brief Stats dialog click button slot
    void on_btnStats_clicked();

    /// @brief Controller dialog click button slot
    void on_btnController_clicked();

    /// @brief Serial monitor dialog click button slot
    void on_serial_monitor_clicked();

    // simulator button slot
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
