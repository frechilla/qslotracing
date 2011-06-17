#include <QtCore/QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qmessagebox.h>

#define STATS_TIMER_EXPIRE_MSEC 10000 // 10 seconds

// start the testing. a byte will be read each 25ms
#define ASCII_SNIFFER_DELAY 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialSniffer(parent),
    m_asciiSniffer(parent),
    m_msgFactory(parent),
    m_scxAnalyzer(parent),
    m_statsTimer(parent),
    m_PlayersBestTimes(6),
    m_PlayerLastLapTime(6),
    m_PlayerCrossings(6)
{
    ui->setupUi(this);
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 lightgreen, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");

    // Initialize protocol stacks (basically connect slots to signals)
    InitializeProtoStack();

    // connect the stats timer to its handler and start it
    this->connect(&m_statsTimer, SIGNAL(timeout()), SLOT(UpdateStats()));
    m_statsTimer.start(STATS_TIMER_EXPIRE_MSEC);

    // Initialize window widgets backgrounds
    InitFuelBackground();

    // Default configuration
    ConfigurePlayer1("", 0, -1);
    ConfigurePlayer2("", 0, -1);
    ConfigurePlayer3("", 0, -1);
    ConfigurePlayer4("", 0, -1);
    ConfigurePlayer5("", 0, -1);
    ConfigurePlayer6("", 0, -1);

    // Initialize status frame
    InitStatusFrame();

    // Players best times initialization
    m_PlayersBestTimes[0] = 0;
    m_PlayersBestTimes[1] = 0;
    m_PlayersBestTimes[2] = 0;
    m_PlayersBestTimes[3] = 0;
    m_PlayersBestTimes[4] = 0;
    m_PlayersBestTimes[5] = 0;

    // Initialize last players lap time
    m_PlayerLastLapTime[0] = 0;
    m_PlayerLastLapTime[1] = 0;
    m_PlayerLastLapTime[2] = 0;
    m_PlayerLastLapTime[3] = 0;
    m_PlayerLastLapTime[4] = 0;
    m_PlayerLastLapTime[5] = 0;

    // Initialize player number of crossings
    m_PlayerCrossings[0] = 0;
    m_PlayerCrossings[1] = 0;
    m_PlayerCrossings[2] = 0;
    m_PlayerCrossings[3] = 0;
    m_PlayerCrossings[4] = 0;
    m_PlayerCrossings[5] = 0;

    // Initialize current player with best race time
    m_CurPlayerBestLapTime = e_QSlotRacingNoPlayer;

    // Initialize timing strings
    InitTimingStrings();
}

MainWindow::~MainWindow()
{
    // tell the serial sniffer thread to die and wait for it
    m_serialSniffer.Join();

    delete ui;
}

void MainWindow::InitializeProtoStack()
{
///////////
//TODO This is DEBUG code. File(s) should be read from some sort of GUI element
#if defined (Q_OS_UNIX)
    m_asciiSniffer.AddAsciiFile("/home/frechilla/workspace/headless/QSlotRacing/doc/ascii-sniffer-example");
#elif defined (Q_OS_WIN)
    m_asciiSniffer.AddAsciiFile("C:\\file");
#endif
//
///////////

    // SCXMsgFactory will be processing all bytes sniffed
    // by the sniffers (both serial and ascii)


    m_msgFactory.connect(&m_serialSniffer,
                       SIGNAL(DataRead(QByteArray)),
                       SLOT(Parse(QByteArray)));


    // SCXMsgFactory will process all bytes snifferd from ascii file sniffer.
    m_msgFactory.connect(&m_asciiSniffer,
                       SIGNAL(DataRead(QByteArray)),
                       SLOT(Parse(QByteArray)));


    // all sniffed bytes will also be notified into the serial monitor window
    // again, for both serial and ascii sniffers
    m_monitor.connect(&m_serialSniffer,
                      SIGNAL(DataRead(QByteArray)),
                      SLOT(ReadData(QByteArray)));


    m_monitor.connect(&m_asciiSniffer,
                      SIGNAL(DataRead(QByteArray)),
                      SLOT(ReadData(QByteArray)));


    // Message factory output will be sent into the proto analyzer
    m_scxAnalyzer.connect(&m_msgFactory,
                        SIGNAL(MsgParsed(QSharedPointer<QSlotRacingMsg>)),
                        SLOT(ProcessMsg(QSharedPointer<QSlotRacingMsg>)));


    // Events coming from the SCXProtoAnalyzer will be processed by this window
    this->connect(&m_scxAnalyzer,
                  SIGNAL(ProtoEvent(QSharedPointer<QSlotRacingEvent>)),
                  SLOT(ProcessEvent(QSharedPointer<QSlotRacingEvent>)));

    // Event of protocol synced
    this->connect(&m_msgFactory,
                  SIGNAL(ProtocolSynced(bool)),
                  SLOT(UpdateSynchroStatus(bool)));

/*
    //TODO DEBUG code
    // serial sniffer is also connected to MainWindow::slotRead
    // it should be deleted at some point
    this->connect(&m_serialSniffer,
                  SIGNAL(DataRead(QByteArray)),
                  SLOT(slotRead(QByteArray)));
                  */
}

void MainWindow::ProcessEvent(QSharedPointer<QSlotRacingEvent> a_event)
{
    switch (a_event->EventType())
    {
    case e_QSlotRacingEvent_Fuel:
    {
        QSharedPointer<QSlotRacingEventFuel> fuelEvent =
                a_event.staticCast<QSlotRacingEventFuel>();

        // GetPlayersFuel returns a value from 0 to 8
        quint8 value;
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer1);
        SetCar1Fuel(value);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer2);
        SetCar2Fuel(value);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer3);
        SetCar3Fuel(value);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer4);
        SetCar4Fuel(value);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer5);
        SetCar5Fuel(value);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer6);
        SetCar6Fuel(value);

        break;
    } // case e_QSlotRacingEvent_Fuel
    case e_QSlotRacingEvent_Controller:
    {
        QSharedPointer<QSlotRacingEventController> controllerEvent =
                a_event.staticCast<QSlotRacingEventController>();

        quint8 retValue;
        bool   lights = false;
        bool   lane_change = false;
        bool   valid = false;
        quint8 speed = 0;

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer1, valid, lights, lane_change, speed);
        if (valid == true)
        {
            SetController1(lights, lane_change, speed);
        }

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer2, valid, lights, lane_change, speed);
        if (valid == true)
        {
            SetController2(lights, lane_change, speed);
        }

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer3, valid, lights, lane_change, speed);
        if (valid == true)
        {
            SetController3(lights, lane_change, speed);
        }

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer4, valid, lights, lane_change, speed);
        if (valid == true)
        {
            SetController4(lights, lane_change, speed);
        }

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer5, valid, lights, lane_change, speed);
        if (valid == true)
        {
            SetController5(lights, lane_change, speed);
        }

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer6, valid, lights, lane_change, speed);
        if (valid == true)
        {
            SetController6(lights, lane_change, speed);
        }
        break;
    } // case e_QSlotRacingEvent_Controller
    case e_QSlotRacingEvent_Ranking:
    {
        QSharedPointer<QSlotRacingEventRanking> rankingEvent =
                a_event.staticCast<QSlotRacingEventRanking>();

        quint8 pos;
        quint8 retValue;
        bool   moreThan15;
        bool   carFlag;
        quint8 lapsBehind;

        // Get data from position 1
        retValue = rankingEvent->GetRankingDataByCar(0, pos, moreThan15, carFlag, lapsBehind);

        // Update position from retrieved carId
        UpdateCarPosition(1, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 2
        retValue = rankingEvent->GetRankingDataByCar(1, pos, moreThan15, carFlag, lapsBehind);

        // Update position from retrieved carId
        UpdateCarPosition(2, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 3
        retValue = rankingEvent->GetRankingDataByCar(2, pos, moreThan15, carFlag, lapsBehind);

        // Update position from retrieved carId
        UpdateCarPosition(3, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 4
        retValue = rankingEvent->GetRankingDataByCar(3, pos, moreThan15, carFlag, lapsBehind);

        // Update position from retrieved carId
        UpdateCarPosition(4, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 5
        retValue = rankingEvent->GetRankingDataByCar(4, pos, moreThan15, carFlag, lapsBehind);

        // Update position from retrieved carId
        UpdateCarPosition(5, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 6
        retValue = rankingEvent->GetRankingDataByCar(5, pos, moreThan15, carFlag, lapsBehind);

        // Update position from retrieved carId
        UpdateCarPosition(6, pos, carFlag, moreThan15, lapsBehind);

        break;
    } // case e_QSlotRacingEvent_Ranking
    case e_QSlotRacingEvent_LapCounter:
    {
        QSharedPointer<QSlotRacingEventLapCounter> lapCounterEvent =
                a_event.staticCast<QSlotRacingEventLapCounter>();

        quint8  retValue;

        // Get event laps
        retValue = lapCounterEvent->GetLapCounterData(m_CountingDir, m_LapsCounter);

        // Update laps for all drivers
        UpdateLaps();
        break;
    }
    case e_QSlotRacingEvent_Lap:
    {
        QSharedPointer<QSlotRacingEventLap> lapEvent =
                a_event.staticCast<QSlotRacingEventLap>();

        QSlotRacingPlayer_t   player;
        qint32                crossings;
        quint32               time;
        QString               text;
        QString               timelap;
        char                  data[20];
        bool                  bestOwnTime;
        bool                  bestRaceLapTime;

        // Initialization
        memset(data, 0, 20);
        bestOwnTime = false;
        bestRaceLapTime = false;

        // Get event data
        player = lapEvent->GetWho();
        crossings = lapEvent->GetCrossingTimes();
        time = lapEvent->GetLapMillis();

        // Format crossings string
        crossings = crossings - 1; // Initial crossing must not be counted
        sprintf(data, "%d/%d", crossings,m_LapsCounter);
        text = QString::fromLocal8Bit(data);

        if (player == e_QSlotRacingPlayer1)
        {
            player = e_QSlotRacingPlayer1;
        }

        // Update race best lap time
        UpdateRaceBestLapTime(player,time, crossings);

        // Update player current lap time
        UpdatePlayerLapTime(player, time, crossings);

        switch(player)
        {
        case e_QSlotRacingPlayer1:
            {
                // Set laps counter display
                ui->editLaps1->setText(text);

                m_PlayerCrossings[0] = crossings;

                break;
            }
        case e_QSlotRacingPlayer2:
            {
                // Set laps counter display
                ui->editLaps2->setText(text);

                m_PlayerCrossings[1] = crossings;

                break;
            }
        case e_QSlotRacingPlayer3:
            {
                // Set laps counter display
                ui->editLaps3->setText(text);

                m_PlayerCrossings[2] = crossings;

                break;
            }
        case e_QSlotRacingPlayer4:
            {
                // Set laps counter display
                ui->editLaps4->setText(text);

                m_PlayerCrossings[3] = crossings;

                break;
            }
        case e_QSlotRacingPlayer5:
            {
                // Set laps counter display
                ui->editLaps5->setText(text);

                m_PlayerCrossings[4] = crossings;

                break;
            }
        case e_QSlotRacingPlayer6:
            {
                // Set laps counter display
                ui->editLaps6->setText(text);

                m_PlayerCrossings[5] = crossings;

                break;
            }
        case e_QSlotRacingNoPlayer:
        default:
            {
                // do nothing
                break;
            }
        }


        break;
    }
    default:
    {
        // highly unexpected
        Q_ASSERT(0);
    }

    } // switch (a_event->EventType())

    //TODO remove
    //QMessageBox msg;
    //msg.exec();

}

void MainWindow::on_BtnConfigure_clicked()
{
    QString    PlayerName;
    bool       PlayerFlag;
    int        PlayerCar;
    bool       configured;
    QString    port;

    // Show configuration dialog
    m_config.exec();

    m_config.GetConfigured(configured);

    if (configured == true)
    {
        // Serial port initialization
        m_config.GetSerialPort(port);
        OpenSerialPort(port);


        // Get configuration data
        m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 1);
        //qDebug()<<"Player 1:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

        // Configure players
        ConfigurePlayer1(PlayerName, PlayerFlag, PlayerCar);

        m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 2);
        //qDebug()<<"Player 2:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

        // Configure players
        ConfigurePlayer2(PlayerName, PlayerFlag, PlayerCar);

        m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 3);
        //qDebug()<<"Player 3:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

        // Configure players
        ConfigurePlayer3(PlayerName, PlayerFlag, PlayerCar);

        m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 4);
        //qDebug()<<"Player 4:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

        // Configure players
        ConfigurePlayer4(PlayerName, PlayerFlag, PlayerCar);

        m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 5);
        //qDebug()<<"Player 5:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

        // Configure players
        ConfigurePlayer5(PlayerName, PlayerFlag, PlayerCar);

        m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 6);
        //qDebug()<<"Player 6:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

        // Configure players
        ConfigurePlayer6(PlayerName, PlayerFlag, PlayerCar);

        // Update race status
        UpdateRaceStatus(e_QSlotRacingRaceMode);
    }
    else
    {
        qDebug()<<"No se ha configurado";
    }
}

void MainWindow::InitFuelBackground(void)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm1_1->palette();

    // Set background color to black
    palette.setColor( backgroundRole(), Qt::black );

    // Update segments for car 1
    ui->FuelSegm1_1->setPalette( palette );
    ui->FuelSegm1_1->setAutoFillBackground( true );
    ui->FuelSegm1_2->setPalette( palette );
    ui->FuelSegm1_2->setAutoFillBackground( true );
    ui->FuelSegm1_3->setPalette( palette );
    ui->FuelSegm1_3->setAutoFillBackground( true );
    ui->FuelSegm1_4->setPalette( palette );
    ui->FuelSegm1_4->setAutoFillBackground( true );
    ui->FuelSegm1_5->setPalette( palette );
    ui->FuelSegm1_5->setAutoFillBackground( true );
    ui->FuelSegm1_6->setPalette( palette );
    ui->FuelSegm1_6->setAutoFillBackground( true );
    ui->FuelSegm1_7->setPalette( palette );
    ui->FuelSegm1_7->setAutoFillBackground( true );
    ui->FuelSegm1_8->setPalette( palette );
    ui->FuelSegm1_8->setAutoFillBackground( true );

    // Update segments for car 2
    ui->FuelSegm2_1->setPalette( palette );
    ui->FuelSegm2_1->setAutoFillBackground( true );
    ui->FuelSegm2_2->setPalette( palette );
    ui->FuelSegm2_2->setAutoFillBackground( true );
    ui->FuelSegm2_3->setPalette( palette );
    ui->FuelSegm2_3->setAutoFillBackground( true );
    ui->FuelSegm2_4->setPalette( palette );
    ui->FuelSegm2_4->setAutoFillBackground( true );
    ui->FuelSegm2_5->setPalette( palette );
    ui->FuelSegm2_5->setAutoFillBackground( true );
    ui->FuelSegm2_6->setPalette( palette );
    ui->FuelSegm2_6->setAutoFillBackground( true );
    ui->FuelSegm2_7->setPalette( palette );
    ui->FuelSegm2_7->setAutoFillBackground( true );
    ui->FuelSegm2_8->setPalette( palette );
    ui->FuelSegm2_8->setAutoFillBackground( true );

    // Update segments for car 3
    ui->FuelSegm3_1->setPalette( palette );
    ui->FuelSegm3_1->setAutoFillBackground( true );
    ui->FuelSegm3_2->setPalette( palette );
    ui->FuelSegm3_2->setAutoFillBackground( true );
    ui->FuelSegm3_3->setPalette( palette );
    ui->FuelSegm3_3->setAutoFillBackground( true );
    ui->FuelSegm3_4->setPalette( palette );
    ui->FuelSegm3_4->setAutoFillBackground( true );
    ui->FuelSegm3_5->setPalette( palette );
    ui->FuelSegm3_5->setAutoFillBackground( true );
    ui->FuelSegm3_6->setPalette( palette );
    ui->FuelSegm3_6->setAutoFillBackground( true );
    ui->FuelSegm3_7->setPalette( palette );
    ui->FuelSegm3_7->setAutoFillBackground( true );
    ui->FuelSegm3_8->setPalette( palette );
    ui->FuelSegm3_8->setAutoFillBackground( true );

    // Update segments for car 4
    ui->FuelSegm4_1->setPalette( palette );
    ui->FuelSegm4_1->setAutoFillBackground( true );
    ui->FuelSegm4_2->setPalette( palette );
    ui->FuelSegm4_2->setAutoFillBackground( true );
    ui->FuelSegm4_3->setPalette( palette );
    ui->FuelSegm4_3->setAutoFillBackground( true );
    ui->FuelSegm4_4->setPalette( palette );
    ui->FuelSegm4_4->setAutoFillBackground( true );
    ui->FuelSegm4_5->setPalette( palette );
    ui->FuelSegm4_5->setAutoFillBackground( true );
    ui->FuelSegm4_6->setPalette( palette );
    ui->FuelSegm4_6->setAutoFillBackground( true );
    ui->FuelSegm4_7->setPalette( palette );
    ui->FuelSegm4_7->setAutoFillBackground( true );
    ui->FuelSegm4_8->setPalette( palette );
    ui->FuelSegm4_8->setAutoFillBackground( true );

    // Update segments for car 5
    ui->FuelSegm5_1->setPalette( palette );
    ui->FuelSegm5_1->setAutoFillBackground( true );
    ui->FuelSegm5_2->setPalette( palette );
    ui->FuelSegm5_2->setAutoFillBackground( true );
    ui->FuelSegm5_3->setPalette( palette );
    ui->FuelSegm5_3->setAutoFillBackground( true );
    ui->FuelSegm5_4->setPalette( palette );
    ui->FuelSegm5_4->setAutoFillBackground( true );
    ui->FuelSegm5_5->setPalette( palette );
    ui->FuelSegm5_5->setAutoFillBackground( true );
    ui->FuelSegm5_6->setPalette( palette );
    ui->FuelSegm5_6->setAutoFillBackground( true );
    ui->FuelSegm5_7->setPalette( palette );
    ui->FuelSegm5_7->setAutoFillBackground( true );
    ui->FuelSegm5_8->setPalette( palette );
    ui->FuelSegm5_8->setAutoFillBackground( true );

    // Update segments for car 6
    ui->FuelSegm6_1->setPalette( palette );
    ui->FuelSegm6_1->setAutoFillBackground( true );
    ui->FuelSegm6_2->setPalette( palette );
    ui->FuelSegm6_2->setAutoFillBackground( true );
    ui->FuelSegm6_3->setPalette( palette );
    ui->FuelSegm6_3->setAutoFillBackground( true );
    ui->FuelSegm6_4->setPalette( palette );
    ui->FuelSegm6_4->setAutoFillBackground( true );
    ui->FuelSegm6_5->setPalette( palette );
    ui->FuelSegm6_5->setAutoFillBackground( true );
    ui->FuelSegm6_6->setPalette( palette );
    ui->FuelSegm6_6->setAutoFillBackground( true );
    ui->FuelSegm6_7->setPalette( palette );
    ui->FuelSegm6_7->setAutoFillBackground( true );
    ui->FuelSegm6_8->setPalette( palette );
    ui->FuelSegm6_8->setAutoFillBackground( true );
}

void MainWindow::SetCar1Fuel(quint8 value)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm1_1->palette();

    switch(value)
    {
    case 1:
        {
            palette.setColor( backgroundRole(), Qt::red );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 2:
        {
            palette.setColor( backgroundRole(), Qt::yellow );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 3:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 4:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 5:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 6:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 7:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    case 8:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm1_1->setPalette( palette );
            ui->FuelSegm1_1->setAutoFillBackground( true );
            ui->FuelSegm1_2->setPalette( palette );
            ui->FuelSegm1_2->setAutoFillBackground( true );
            ui->FuelSegm1_3->setPalette( palette );
            ui->FuelSegm1_3->setAutoFillBackground( true );
            ui->FuelSegm1_4->setPalette( palette );
            ui->FuelSegm1_4->setAutoFillBackground( true );
            ui->FuelSegm1_5->setPalette( palette );
            ui->FuelSegm1_5->setAutoFillBackground( true );
            ui->FuelSegm1_6->setPalette( palette );
            ui->FuelSegm1_6->setAutoFillBackground( true );
            ui->FuelSegm1_7->setPalette( palette );
            ui->FuelSegm1_7->setAutoFillBackground( true );
            ui->FuelSegm1_8->setPalette( palette );
            ui->FuelSegm1_8->setAutoFillBackground( true );
            break;
        }
    }
}

void MainWindow::SetCar2Fuel(quint8 value)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm2_1->palette();

    switch(value)
    {
    case 1:
        {
            palette.setColor( backgroundRole(), Qt::red );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 2:
        {
            palette.setColor( backgroundRole(), Qt::yellow );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 3:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 4:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 5:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 6:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 7:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    case 8:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm2_1->setPalette( palette );
            ui->FuelSegm2_1->setAutoFillBackground( true );
            ui->FuelSegm2_2->setPalette( palette );
            ui->FuelSegm2_2->setAutoFillBackground( true );
            ui->FuelSegm2_3->setPalette( palette );
            ui->FuelSegm2_3->setAutoFillBackground( true );
            ui->FuelSegm2_4->setPalette( palette );
            ui->FuelSegm2_4->setAutoFillBackground( true );
            ui->FuelSegm2_5->setPalette( palette );
            ui->FuelSegm2_5->setAutoFillBackground( true );
            ui->FuelSegm2_6->setPalette( palette );
            ui->FuelSegm2_6->setAutoFillBackground( true );
            ui->FuelSegm2_7->setPalette( palette );
            ui->FuelSegm2_7->setAutoFillBackground( true );
            ui->FuelSegm2_8->setPalette( palette );
            ui->FuelSegm2_8->setAutoFillBackground( true );
            break;
        }
    }

}

void MainWindow::SetCar3Fuel(quint8 value)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm3_1->palette();

    switch(value)
    {
    case 1:
        {
            palette.setColor( backgroundRole(), Qt::red );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 2:
        {
            palette.setColor( backgroundRole(), Qt::yellow );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 3:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 4:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 5:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 6:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 7:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    case 8:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm3_1->setPalette( palette );
            ui->FuelSegm3_1->setAutoFillBackground( true );
            ui->FuelSegm3_2->setPalette( palette );
            ui->FuelSegm3_2->setAutoFillBackground( true );
            ui->FuelSegm3_3->setPalette( palette );
            ui->FuelSegm3_3->setAutoFillBackground( true );
            ui->FuelSegm3_4->setPalette( palette );
            ui->FuelSegm3_4->setAutoFillBackground( true );
            ui->FuelSegm3_5->setPalette( palette );
            ui->FuelSegm3_5->setAutoFillBackground( true );
            ui->FuelSegm3_6->setPalette( palette );
            ui->FuelSegm3_6->setAutoFillBackground( true );
            ui->FuelSegm3_7->setPalette( palette );
            ui->FuelSegm3_7->setAutoFillBackground( true );
            ui->FuelSegm3_8->setPalette( palette );
            ui->FuelSegm3_8->setAutoFillBackground( true );
            break;
        }
    }
}

void MainWindow::SetCar4Fuel(quint8 value)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm4_1->palette();

    switch(value)
    {
    case 1:
        {
            palette.setColor( backgroundRole(), Qt::red );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 2:
        {
            palette.setColor( backgroundRole(), Qt::yellow );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 3:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 4:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 5:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 6:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 7:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    case 8:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm4_1->setPalette( palette );
            ui->FuelSegm4_1->setAutoFillBackground( true );
            ui->FuelSegm4_2->setPalette( palette );
            ui->FuelSegm4_2->setAutoFillBackground( true );
            ui->FuelSegm4_3->setPalette( palette );
            ui->FuelSegm4_3->setAutoFillBackground( true );
            ui->FuelSegm4_4->setPalette( palette );
            ui->FuelSegm4_4->setAutoFillBackground( true );
            ui->FuelSegm4_5->setPalette( palette );
            ui->FuelSegm4_5->setAutoFillBackground( true );
            ui->FuelSegm4_6->setPalette( palette );
            ui->FuelSegm4_6->setAutoFillBackground( true );
            ui->FuelSegm4_7->setPalette( palette );
            ui->FuelSegm4_7->setAutoFillBackground( true );
            ui->FuelSegm4_8->setPalette( palette );
            ui->FuelSegm4_8->setAutoFillBackground( true );
            break;
        }
    }
}

void MainWindow::SetCar5Fuel(quint8 value)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm5_1->palette();

    switch(value)
    {
    case 1:
        {
            palette.setColor( backgroundRole(), Qt::red );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 2:
        {
            palette.setColor( backgroundRole(), Qt::yellow );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 3:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 4:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 5:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 6:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 7:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    case 8:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm5_1->setPalette( palette );
            ui->FuelSegm5_1->setAutoFillBackground( true );
            ui->FuelSegm5_2->setPalette( palette );
            ui->FuelSegm5_2->setAutoFillBackground( true );
            ui->FuelSegm5_3->setPalette( palette );
            ui->FuelSegm5_3->setAutoFillBackground( true );
            ui->FuelSegm5_4->setPalette( palette );
            ui->FuelSegm5_4->setAutoFillBackground( true );
            ui->FuelSegm5_5->setPalette( palette );
            ui->FuelSegm5_5->setAutoFillBackground( true );
            ui->FuelSegm5_6->setPalette( palette );
            ui->FuelSegm5_6->setAutoFillBackground( true );
            ui->FuelSegm5_7->setPalette( palette );
            ui->FuelSegm5_7->setAutoFillBackground( true );
            ui->FuelSegm5_8->setPalette( palette );
            ui->FuelSegm5_8->setAutoFillBackground( true );
            break;
        }
    }
}

void MainWindow::SetCar6Fuel(quint8 value)
{
    QPalette palette;

    // Get default palette for all devices
    ui->FuelSegm6_1->palette();

    switch(value)
    {
    case 1:
        {
            palette.setColor( backgroundRole(), Qt::red );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 2:
        {
            palette.setColor( backgroundRole(), Qt::yellow );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 3:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 4:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 5:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 6:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 7:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            palette.setColor( backgroundRole(), Qt::black );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    case 8:
        {
            palette.setColor( backgroundRole(), Qt::green );
            ui->FuelSegm6_1->setPalette( palette );
            ui->FuelSegm6_1->setAutoFillBackground( true );
            ui->FuelSegm6_2->setPalette( palette );
            ui->FuelSegm6_2->setAutoFillBackground( true );
            ui->FuelSegm6_3->setPalette( palette );
            ui->FuelSegm6_3->setAutoFillBackground( true );
            ui->FuelSegm6_4->setPalette( palette );
            ui->FuelSegm6_4->setAutoFillBackground( true );
            ui->FuelSegm6_5->setPalette( palette );
            ui->FuelSegm6_5->setAutoFillBackground( true );
            ui->FuelSegm6_6->setPalette( palette );
            ui->FuelSegm6_6->setAutoFillBackground( true );
            ui->FuelSegm6_7->setPalette( palette );
            ui->FuelSegm6_7->setAutoFillBackground( true );
            ui->FuelSegm6_8->setPalette( palette );
            ui->FuelSegm6_8->setAutoFillBackground( true );
            break;
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    // objects have been already initialized in InitializeProtoStack

    m_asciiSniffer.Start(ASCII_SNIFFER_DELAY);

    //TODO would be nice to be able to stop the ascii sniffer

    //TODO this is some good stuff to see how much data we are discarding
    // it should used to fill up some textbox somewhere
    //std::cout << "Messages discarded: "
    //          << msgFactory.GetBytesDiscardedCount() << std::endl;
}

void MainWindow::OpenSerialPort(QString port)
{
    // set settings to open the serial port
    m_serialSniffer.SetDeviceName(port);
    m_serialSniffer.SetBaudRate(AbstractSerial::BaudRate115200);
    m_serialSniffer.SetDataBits(AbstractSerial::DataBits8);
    m_serialSniffer.SetParity(AbstractSerial::ParityNone);
    m_serialSniffer.SetStopBits(AbstractSerial::StopBits1);
    m_serialSniffer.SetFlowControl(AbstractSerial::FlowControlOff);
    
    // Open it
    m_serialSniffer.OpenSerial();

    // allow the thread to start sniffing the interface
    m_serialSniffer.Resume();
}

void MainWindow::on_serial_monitor_clicked()
{
    //static SerialMonitor *diag = new SerialMonitor(this);
    m_monitor.show();
}

void MainWindow::ConfigurePlayer1(QString player, bool flag, int car)
{
    //qDebug() <<"Configuring player 1: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps1->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos1->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos1->setText("0");

        // Display player name
        ui->labelName1->setText(player);

        // Set Default laps
        ui->editLaps1->setText("0/0");

        // Set player enable flag
        m_PlayersConfigured[0] = true;

        // Display player car
        switch (car)
        {
        case 0:
            {
                ui->car1->setPixmap(QPixmap(":/pics/ferrari"));
                ui->car1->show();
                break;
            }
        case 1:
            {
                ui->car1->setPixmap(QPixmap(":/pics/renault"));
                ui->car1->show();
                break;
            }
        case 2:
            {
                ui->car1->setPixmap(QPixmap(":/pics/mclaren"));
                ui->car1->show();
                break;
            }
        case 3:
            {
                ui->car1->setPixmap(QPixmap(":/pics/williams"));
                ui->car1->show();
                break;
            }
        }
    }
    else
    {
        // Player is not enabled
        ui->car1->setPixmap(QPixmap(":/pics/empty"));
        ui->car2->show();

        // Disable lap counter
        ui->editLaps1->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editPos1->setStyleSheet("background-color: rgb(220, 220, 220);");

        ui->editLaps1->setEnabled(false);
        ui->editLaps1->setText("");
        ui->editPos1->setEnabled(false);
        ui->editPos1->setText("");

        m_PlayersConfigured[0] = false;
    }
}

void MainWindow::ConfigurePlayer2(QString player, bool flag, int car)
{
    //qDebug() <<"Configuring player 2: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps2->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos2->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos2->setText("0");

        // Display player name
        ui->labelName2->setText(player);

        // Set Default laps
        ui->editLaps2->setText("0/0");

        // Set player enable flag
        m_PlayersConfigured[1] = true;

        // Display player car
        switch (car)
        {
        case 0:
            {
                ui->car2->setPixmap(QPixmap(":/pics/ferrari"));
                ui->car2->show();
                break;
            }
        case 1:
            {
                ui->car2->setPixmap(QPixmap(":/pics/renault"));
                ui->car2->show();
                break;
            }
        case 2:
            {
                ui->car2->setPixmap(QPixmap(":/pics/mclaren"));
                ui->car2->show();
                break;
            }
        case 3:
            {
                ui->car2->setPixmap(QPixmap(":/pics/williams"));
                ui->car2->show();
                break;
            }
        }
    }
    else
    {
        // Player is not enabled
        ui->car2->setPixmap(QPixmap(":/pics/empty"));
        ui->car2->show();
        ui->editLaps2->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editPos2->setStyleSheet("background-color: rgb(220, 220, 220);");

        ui->editLaps2->setEnabled(false);
        ui->editLaps2->setText("");
        ui->editPos2->setEnabled(false);
        ui->editPos2->setText("");

        m_PlayersConfigured[1] = false;
    }
}

void MainWindow::ConfigurePlayer3(QString player, bool flag, int car)
{
    //qDebug() <<"Configuring player 3: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps3->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos3->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos3->setText("0");

        // Display player name
        ui->labelName3->setText(player);

        // Set Default laps
        ui->editLaps3->setText("0/0");

        // Set player enable flag
        m_PlayersConfigured[2] = true;

        // Display player car
        switch (car)
        {
        case 0:
            {
                ui->car3->setPixmap(QPixmap(":/pics/ferrari"));
                ui->car3->show();
                break;
            }
        case 1:
            {
                ui->car3->setPixmap(QPixmap(":/pics/renault"));
                ui->car3->show();
                break;
            }
        case 2:
            {
                ui->car3->setPixmap(QPixmap(":/pics/mclaren"));
                ui->car3->show();
                break;
            }
        case 3:
            {
                ui->car3->setPixmap(QPixmap(":/pics/williams"));
                ui->car3->show();
                break;
            }
        }
    }
    else
    {
        // Player is not enabled
        ui->car3->setPixmap(QPixmap(":/pics/empty"));
        ui->car3->show();
        ui->editLaps3->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editPos3->setStyleSheet("background-color: rgb(220, 220, 220);");

        ui->editLaps3->setEnabled(false);
        ui->editLaps3->setText("");
        ui->editPos3->setEnabled(false);
        ui->editPos3->setText("");

        m_PlayersConfigured[2] = false;
    }
}


void MainWindow::ConfigurePlayer4(QString player, bool flag, int car)
{
    //qDebug() <<"Configuring player 4: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps4->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos4->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos4->setText("0");

        // Display player name
        ui->labelName4->setText(player);

        // Set Default laps
        ui->editLaps4->setText("0/0");

        // Set player enable flag
        m_PlayersConfigured[3] = true;

        // Display player car
        switch (car)
        {
        case 0:
            {
                ui->car4->setPixmap(QPixmap(":/pics/ferrari"));
                ui->car4->show();
                break;
            }
        case 1:
            {
                ui->car4->setPixmap(QPixmap(":/pics/renault"));
                ui->car4->show();
                break;
            }
        case 2:
            {
                ui->car4->setPixmap(QPixmap(":/pics/mclaren"));
                ui->car4->show();
                break;
            }
        case 3:
            {
                ui->car4->setPixmap(QPixmap(":/pics/williams"));
                ui->car4->show();
                break;
            }
        }
    }
    else
    {
        // Player is not enabled
        ui->car4->setPixmap(QPixmap(":/pics/empty"));
        ui->car4->show();

        ui->editLaps4->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editPos4->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editLaps4->setEnabled(false);
        ui->editLaps4->setText("");
        ui->editPos4->setEnabled(false);
        ui->editPos4->setText("");
        m_PlayersConfigured[3] = false;
    }
}

void MainWindow::ConfigurePlayer5(QString player, bool flag, int car)
{
    //qDebug() <<"Configuring player 5: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps5->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos5->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos5->setText("0");

        // Display player name
        ui->labelName5->setText(player);

        // Set Default laps
        ui->editLaps5->setText("0/0");

        // Set player enable flag
        m_PlayersConfigured[4] = true;

        // Display player car
        switch (car)
        {
        case 0:
            {
                ui->car5->setPixmap(QPixmap(":/pics/ferrari"));
                ui->car5->show();
                break;
            }
        case 1:
            {
                ui->car5->setPixmap(QPixmap(":/pics/renault"));
                ui->car5->show();
                break;
            }
        case 2:
            {
                ui->car5->setPixmap(QPixmap(":/pics/mclaren"));
                ui->car5->show();
                break;
            }
        case 3:
            {
                ui->car5->setPixmap(QPixmap(":/pics/williams"));
                ui->car5->show();
                break;
            }
        }
    }
    else
    {
        // Player is not enabled
        ui->car5->setPixmap(QPixmap(":/pics/empty"));
        ui->car5->show();

        ui->editLaps5->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editPos5->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editLaps5->setEnabled(false);
        ui->editLaps5->setText("");
        ui->editPos5->setEnabled(false);
        ui->editPos5->setText("");
        m_PlayersConfigured[4] = false;
    }
}

void MainWindow::ConfigurePlayer6(QString player, bool flag, int car)
{
    //qDebug() <<"Configuring player 6: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps6->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos6->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos6->setText("0");

        // Display player name
        ui->labelName6->setText(player);

        // Set Default laps
        ui->editLaps6->setText("0/0");

        // Set player enable flag
        m_PlayersConfigured[5] = true;

        // Display player car
        switch (car)
        {
        case 0:
            {
                ui->car6->setPixmap(QPixmap(":/pics/ferrari"));
                ui->car6->show();
                break;
            }
        case 1:
            {
                ui->car6->setPixmap(QPixmap(":/pics/renault"));
                ui->car6->show();
                break;
            }
        case 2:
            {
                ui->car6->setPixmap(QPixmap(":/pics/mclaren"));
                ui->car6->show();
                break;
            }
        case 3:
            {
                ui->car6->setPixmap(QPixmap(":/pics/williams"));
                ui->car6->show();
                break;
            }
        default:
            {
                break;
            }
        }
    }
    else
    {
        // Player is not enabled
        ui->car6->setPixmap(QPixmap(":/pics/empty"));
        ui->car6->show();

        ui->editLaps6->setStyleSheet("background-color: rgb(220, 220, 220);");
        ui->editPos6->setStyleSheet("background-color: rgb(220, 220, 220);");

        ui->editLaps6->setEnabled(false);
        ui->editLaps6->setText("");
        ui->editPos6->setEnabled(false);
        ui->editPos6->setText("");
        m_PlayersConfigured[5] = false;
    }
}

void MainWindow::on_btnController_clicked()
{
    m_controller.show();
}

void MainWindow::SetController1(bool lights, bool lane_change, quint8 speed)
{
    m_controller.SetGas1(speed);
    m_controller.SetChange1(lane_change);
}

void MainWindow::SetController2(bool lights, bool lane_change, quint8 speed)
{
    m_controller.SetGas2(speed);
    m_controller.SetChange2(lane_change);
}

void MainWindow::SetController3(bool lights, bool lane_change, quint8 speed)
{
    m_controller.SetGas3(speed);
    m_controller.SetChange3(lane_change);
}

void MainWindow::SetController4(bool lights, bool lane_change, quint8 speed)
{
    m_controller.SetGas4(speed);
    m_controller.SetChange4(lane_change);
}

void MainWindow::SetController5(bool lights, bool lane_change, quint8 speed)
{
    m_controller.SetGas5(speed);
    m_controller.SetChange5(lane_change);
}

void MainWindow::SetController6(bool lights, bool lane_change, quint8 speed)
{
    m_controller.SetGas6(speed);
    m_controller.SetChange6(lane_change);
}

void MainWindow::on_btnStats_clicked()
{
    // TODO: Use http://qwt.sourceforge.net/ for creating graphs
    m_statsdlg.show();
}

void MainWindow::UpdateCarPosition(quint8 carId, quint8 pos, bool carFlag, bool moreThan15, quint8 lapsBehind)
{
    QString    data;
    char       posStr[20];

    // Initialization
    memset(posStr, 0, 20);

    // Check if car is enabled
    if (m_PlayersConfigured[carId -1] == true)
    {
        switch(carId)
        {
        case 1:
            {
                ui->editPos1->setStyleSheet("color: rgb(0, 0, 0);");
                // Check position status
                if (carFlag == false)
                {
                    // No car at this position
                    ui->editPos1->setText("E");
                }
                else if (moreThan15 == true)
                {
                    ui->editPos1->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->editPos1->setText("+15");
                }
                else if (lapsBehind > 0)
                {
                    sprintf(posStr, "+%d", lapsBehind);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos1->setText(data);
                }
                else
                {
                    if ((pos >= 0) && (pos <= 6))
                    {
                        sprintf(posStr, "%d", pos);
                        data = QString::fromLocal8Bit(posStr);
                        ui->editPos1->setText(data);
                    }
                }
                break;
            }
        case 2:
            {
                ui->editPos2->setStyleSheet("color: rgb(0, 0, 0);");
                // Check position status
                if (carFlag == false)
                {
                    // No car at this position
                    ui->editPos2->setText("E");
                }
                else if (moreThan15 == true)
                {
                    ui->editPos2->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->editPos2->setText("+15");
                }
                else if (lapsBehind > 0)
                {
                    sprintf(posStr, "+%d", lapsBehind);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos2->setText(data);
                }
                else
                {
                    sprintf(posStr, "%d", pos);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos2->setText(data);
                }
                break;
            }
        case 3:
            {
                ui->editPos3->setStyleSheet("color: rgb(0, 0, 0);");
                // Check position status
                if (carFlag == false)
                {
                    // No car at this position
                    ui->editPos3->setText("E");
                }
                else if (moreThan15 == true)
                {
                    ui->editPos3->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->editPos3->setText("+15");
                }
                else if (lapsBehind > 0)
                {
                    sprintf(posStr, "+%d", lapsBehind);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos3->setText(data);
                }
                else
                {
                    sprintf(posStr, "%d", pos);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos3->setText(data);
                }
                break;
            }
        case 4:
            {
                ui->editPos4->setStyleSheet("color: rgb(0, 0, 0);");
                // Check position status
                if (carFlag == false)
                {
                    // No car at this position
                    ui->editPos4->setText("E");
                }
                else if (moreThan15 == true)
                {
                    ui->editPos4->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->editPos4->setText("+15");
                }
                else if (lapsBehind > 0)
                {
                    sprintf(posStr, "+%d", lapsBehind);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos4->setText(data);
                }
                else
                {
                    sprintf(posStr, "%d", pos);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos4->setText(data);
                }
                break;
            }
        case 5:
            {
                ui->editPos5->setStyleSheet("color: rgb(0, 0, 0);");
                // Check position status
                if (carFlag == false)
                {
                    // No car at this position
                    ui->editPos5->setText("E");
                }
                else if (moreThan15 == true)
                {
                    ui->editPos5->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->editPos5->setText("+15");
                }
                else if (lapsBehind > 0)
                {
                    sprintf(posStr, "+%d", lapsBehind);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos5->setText(data);
                }
                else
                {
                    sprintf(posStr, "%d", pos);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos5->setText(data);
                }
                break;
            }
        case 6:
            {
                ui->editPos6->setStyleSheet("color: rgb(0, 0, 0);");
                // Check position status
                if (carFlag == false)
                {
                    // No car at this position
                    ui->editPos6->setText("E");
                }
                else if (moreThan15 == true)
                {
                    ui->editPos6->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->editPos6->setText("+15");
                }
                else if (lapsBehind > 0)
                {
                    sprintf(posStr, "+%d", lapsBehind);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos6->setText(data);
                }
                else
                {
                    sprintf(posStr, "%d", pos);
                    data = QString::fromLocal8Bit(posStr);
                    ui->editPos6->setText(data);
                }
                break;
            }
        }
    }
    else
    {
        // Do nothing
    }
}

void MainWindow::UpdateLaps()
{
    QString    text;
    char       data[10];

    // Initialization
    memset(data, 0, 10);

    // Format string
    sprintf(data, "0/%d", m_LapsCounter);
    text = QString::fromLocal8Bit(data);

    // Display data for all configured drivers
    if (m_PlayersConfigured[0] == true)
    {
        ui->editLaps1->setText(text);
    }
    if (m_PlayersConfigured[1] == true)
    {
        ui->editLaps2->setText(text);
    }
    if (m_PlayersConfigured[2] == true)
    {
        ui->editLaps3->setText(text);
    }
    if (m_PlayersConfigured[3] == true)
    {
        ui->editLaps4->setText(text);
    }
    if (m_PlayersConfigured[4] == true)
    {
        ui->editLaps5->setText(text);
    }
    if (m_PlayersConfigured[5] == true)
    {
        ui->editLaps6->setText(text);
    }
}

void MainWindow::UpdateStats()
{
    const QSlotRacingStatsCounter<SCXMsgFactory::eStatEntry_Count>& msgFactoryStats =
            m_msgFactory.GetStatCounters();

    qDebug("STATS: SCX MSG Factory:");
    for (qint32 i = 0; i < SCXMsgFactory::eStatEntry_Count; i++)
    {
        qDebug() << "  "
                 << msgFactoryStats.GetEntryTitle(i)
                 << ": "
                 << msgFactoryStats.GetEntryValue(i);
    }

    const QSlotRacingStatsCounter<SCXProtoAnalyzer::eStatEntry_Count>& scxAnalyzerStats =
            m_scxAnalyzer.GetStatCounters();

    qDebug("STATS: SCX Analyzer:");
    for (qint32 i = 0; i < SCXProtoAnalyzer::eStatEntry_Count; i++)
    {
        qDebug() << "  "
                 << scxAnalyzerStats.GetEntryTitle(i)
                 << ": "
                 << scxAnalyzerStats.GetEntryValue(i);
    }
}

void MainWindow::InitStatusFrame()
{
    // Initialize synchro status
    ui->labelSynchro->setPixmap(QPixmap(QString::fromUtf8(":/pics/red_ball")));

    UpdateRaceStatus(e_QSlotRacingConfigMode);
}

void MainWindow::UpdateSynchroStatus(bool bSynchro)
{
    // Check synchro status flag
    if (bSynchro == true)
    {
        qDebug()<<"poner verde";
        // Update to green status
        ui->labelSynchro->setPixmap(QPixmap(QString::fromUtf8(":/pics/green_ball")));
    }
    else
    {
        qDebug()<<"poner rojo";
        // Update to red status
        ui->labelSynchro->setPixmap(QPixmap(QString::fromUtf8(":/pics/red_ball")));
    }
}

void MainWindow::UpdateRaceStatus(QSlotRacingRaceStatusType_t status)
{
    // Check race status
    if (status == e_QSlotRacingConfigMode)
    {
        // Waiting for configuration status
        ui->labelRace->setPixmap(QPixmap(QString::fromUtf8(":/pics/settings_status")));
    }
    else if (status == e_QSlotRacingRaceMode)
    {
        // Waiting for race status
        ui->labelRace->setPixmap(QPixmap(QString::fromUtf8(":/pics/race_flag_status")));
    }
    else if (status == e_QSlotRacingQualyMode)
    {
        // Waiting for qualifying status
        ui->labelRace->setPixmap(QPixmap(QString::fromUtf8(":/pics/crono_status")));
    }
}

void MainWindow::GetStringFromTime(QString *timestr, quint32 time)
{
    quint8    mins;
    quint8    secs;
    quint16   ms;
    quint32   temp_time;
    char      datatime[50];

    // Initialization
    mins = 0;
    secs = 0;
    ms = 0;
    memset(datatime, 0, 50);
    temp_time = time;

    // Calculate mins
    mins = (quint8)(temp_time / 60000);

    if (mins > 0)
    {
        temp_time = temp_time - (60000 * mins);
    }

    // Calculate secs
    secs = (quint8)(temp_time / 1000);

    if (secs > 0)
    {
        temp_time = temp_time - (secs * 1000);
    }

    // Calculate ms
    ms = (quint16)temp_time;

    sprintf(datatime, "%02d:%02d.%03d", mins, secs, ms);
    *timestr = QString::fromLocal8Bit(datatime);
}

void MainWindow::InitTimingStrings()
{
    ui->labelTime1->setText("--:--.---");
    ui->labelTime2->setText("--:--.---");
    ui->labelTime3->setText("--:--.---");
    ui->labelTime4->setText("--:--.---");
    ui->labelTime5->setText("--:--.---");
    ui->labelTime6->setText("--:--.---");

    ui->labelBest1->setText("--:--.---");
    ui->labelBest2->setText("--:--.---");
    ui->labelBest3->setText("--:--.---");
    ui->labelBest4->setText("--:--.---");
    ui->labelBest5->setText("--:--.---");
    ui->labelBest6->setText("--:--.---");
}

void MainWindow::UpdateRaceBestLapTime(QSlotRacingPlayer_t player, quint32 curtime, quint32 crossing)
{
    QString    timestr;
    QString    qStyleString;
    bool       newCrossing;
    bool       newBestLapTime;
    bool       isPlayerFasterLap;

    // Check for a new crossing
    newCrossing = IsNewCrossing(player, crossing);

    if (newCrossing == true)
    {
        // Check for new best lap time
        newBestLapTime = IsRaceBestLapTime(player, curtime, crossing);

        // Check for player best lap time
        isPlayerFasterLap = IsFasterLap(player, curtime);

        if (newBestLapTime == true)
        {
            // Update background color last best race lap player
            switch (m_CurPlayerBestLapTime)
            {
            case e_QSlotRacingPlayer1:
                {
                    // Format time string
                    GetStringFromTime(&timestr, m_PlayersBestTimes[0]);

                    ui->labelBest1->setStyleSheet("");
                    ui->labelBest1->setText(timestr);

                    // Set background color green, text color white
     //                   ui->labelTime1->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                    break;
                }
            case e_QSlotRacingPlayer2:
                {
                    // Format time string
                    GetStringFromTime(&timestr, m_PlayersBestTimes[1]);

                    ui->labelBest2->setStyleSheet("");
                    ui->labelBest2->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer3:
                {
                    // Format time string
                    GetStringFromTime(&timestr, m_PlayersBestTimes[2]);

                    ui->labelBest3->setStyleSheet("");
                    ui->labelBest3->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer4:
                {
                    // Format time string
                    GetStringFromTime(&timestr, m_PlayersBestTimes[3]);

                    ui->labelBest4->setStyleSheet("");
                    ui->labelBest4->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer5:
                {
                    // Format time string
                    GetStringFromTime(&timestr, m_PlayersBestTimes[4]);

                    ui->labelBest5->setStyleSheet("");
                    ui->labelBest5->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer6:
                {
                    // Format time string
                    GetStringFromTime(&timestr, m_PlayersBestTimes[5]);

                    ui->labelBest6->setStyleSheet("");
                    ui->labelBest6->setText(timestr);
                    break;
                }
            default:
                {
                    break;
                }
            }  // switch last best player

            m_CurPlayerBestLapTime = player;
        }
        else
        {
            // Do nothing. No new best lap time
        }

        // Checke for new best lap time before setting background color
        if (newBestLapTime == true)
        {
            qStyleString = "background-color: rgb(229, 2, 188);color: rgb(255, 255, 255);";
        }
        else
        {
            qStyleString = "";
        }

        if (isPlayerFasterLap == true)
        {
            // Format time string
            GetStringFromTime(&timestr, curtime);

            // Update
            switch (player)
            {
            case e_QSlotRacingPlayer1:
                {
                    ui->labelBest1->setStyleSheet(qStyleString);
                    ui->labelBest1->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer2:
                {
                    ui->labelBest2->setStyleSheet(qStyleString);
                    ui->labelBest2->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer3:
                {
                    ui->labelBest3->setStyleSheet(qStyleString);
                    ui->labelBest3->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer4:
                {
                    ui->labelBest4->setStyleSheet(qStyleString);
                    ui->labelBest4->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer5:
                {
                    ui->labelBest5->setStyleSheet(qStyleString);
                    ui->labelBest5->setText(timestr);
                    break;
                }
            case e_QSlotRacingPlayer6:
                {
                    ui->labelBest6->setStyleSheet(qStyleString);
                    ui->labelBest6->setText(timestr);
                    break;
                }
            default:
                {
                    break;
                }
            }  // switch new best player
        }
        else
        {
            // Do nothing. No personal best lap time so no best race lap time
        }

    }
    else
    {
        // Do nothing. No new crossing detected.
    }
}

bool MainWindow::IsFasterLap(QSlotRacingPlayer_t player, quint32 time)
{
    bool    bIsFaster;

    // Initialization
    bIsFaster = false;

    // Check if player configured and time is better than stored
    if (m_PlayersConfigured[player] &&
        ((time < m_PlayersBestTimes[player]) || (m_PlayersBestTimes[player] == 0)))
    {
        // Update best time
        m_PlayersBestTimes[player] = time;

        // Set faster flag
        bIsFaster = true;
    }

    return bIsFaster;
}

bool MainWindow::IsRaceBestLapTime(QSlotRacingPlayer_t player, quint32 curtime, quint32 crossing)
{
    bool    bIsBestTime;
    bool    newCrossing;

    // Initialization
    bIsBestTime = false;

    // Check for a new crossing
    newCrossing = IsNewCrossing(player, crossing);

    if (newCrossing == true)
    {
        // Check if it's best time
        if (((m_PlayersBestTimes[0] <= curtime) && (m_PlayersBestTimes[0] > 0) && (m_PlayersConfigured[0])) ||
            ((m_PlayersBestTimes[1] <= curtime) && (m_PlayersBestTimes[1] > 0) && (m_PlayersConfigured[1])) ||
            ((m_PlayersBestTimes[2] <= curtime) && (m_PlayersBestTimes[2] > 0) && (m_PlayersConfigured[2])) ||
            ((m_PlayersBestTimes[3] <= curtime) && (m_PlayersBestTimes[3] > 0) && (m_PlayersConfigured[3])) ||
            ((m_PlayersBestTimes[4] <= curtime) && (m_PlayersBestTimes[4] > 0) && (m_PlayersConfigured[4])) ||
            ((m_PlayersBestTimes[5] <= curtime) && (m_PlayersBestTimes[5] > 0) && (m_PlayersConfigured[5])))
        {
            // no better player
            bIsBestTime = false;
        }
        else
        {
            // new best race lap time
            bIsBestTime = true;
        }
    }

    return bIsBestTime;
}


void MainWindow::UpdatePlayerLapTime(QSlotRacingPlayer_t player, quint32 curtime, quint32 crossing)
{
    QString    timelap;
    bool       newCrossing;

    // Check for a new crossing
    newCrossing = IsNewCrossing(player, crossing);

    if (newCrossing == true)
    {
        // Format time string
        GetStringFromTime(&timelap, curtime);

        // Check player
        switch (player)
        {
        case e_QSlotRacingPlayer1:
            {
                // Check last  lap time
                if (curtime < m_PlayerLastLapTime[0] && (m_PlayerLastLapTime[0] > 0))
                {
                    // Faster lap
                    // Set background color green, text color white
                    ui->labelTime1->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                }
                else if (curtime > m_PlayerLastLapTime[0] && (m_PlayerLastLapTime[0] > 0))
                {
                    // Slower lap
                    // Set background color red, text color white
                    ui->labelTime1->setStyleSheet("background-color: rgb(250, 0, 0);color: rgb(255, 255, 255);");
                }
                else
                {
                    // Same time lap
                    // Remove background color
                    ui->labelTime1->setStyleSheet("");
                }
                ui->labelTime1->setText(timelap);

                // Update player last time
                m_PlayerLastLapTime[0] = curtime;
                break;
            }
        case e_QSlotRacingPlayer2:
            {
                // Check last  lap time
                if (curtime < m_PlayerLastLapTime[1] && (m_PlayerLastLapTime[1] > 0))
                {
                    // Faster lap
                    // Set background color green, text color white
                    ui->labelTime2->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                }
                else if (curtime > m_PlayerLastLapTime[1] && (m_PlayerLastLapTime[1] > 0))
                {
                    // Slower lap
                    // Set background color red, text color white
                    ui->labelTime2->setStyleSheet("background-color: rgb(250, 0, 0);color: rgb(255, 255, 255);");
                }
                else
                {
                    // Same time lap
                    // Remove background color
                    ui->labelTime2->setStyleSheet("");
                }
                ui->labelTime2->setText(timelap);

                // Update player last time
                m_PlayerLastLapTime[1] = curtime;
                break;
            }
        case e_QSlotRacingPlayer3:
            {
                // Check last  lap time
                if (curtime < m_PlayerLastLapTime[2] && (m_PlayerLastLapTime[2] > 0))
                {
                    // Faster lap
                    // Set background color green, text color white
                    ui->labelTime3->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                }
                else if (curtime > m_PlayerLastLapTime[2] && (m_PlayerLastLapTime[2] > 0))
                {
                    // Slower lap
                    // Set background color red, text color white
                    ui->labelTime3->setStyleSheet("background-color: rgb(250, 0, 0);color: rgb(255, 255, 255);");
                }
                else
                {
                    // Same time lap
                    // Remove background color
                    ui->labelTime3->setStyleSheet("");
                }
                ui->labelTime3->setText(timelap);

                // Update player last time
                m_PlayerLastLapTime[2] = curtime;
                break;
            }
        case e_QSlotRacingPlayer4:
            {
                // Check last  lap time
                if (curtime < m_PlayerLastLapTime[3] && (m_PlayerLastLapTime[3] > 0))
                {
                    // Faster lap
                    // Set background color green, text color white
                    ui->labelTime4->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                }
                else if (curtime > m_PlayerLastLapTime[3] && (m_PlayerLastLapTime[3] > 0))
                {
                    // Slower lap
                    // Set background color red, text color white
                    ui->labelTime4->setStyleSheet("background-color: rgb(250, 0, 0);color: rgb(255, 255, 255);");
                }
                else
                {
                    // Same time lap
                    // Remove background color
                    ui->labelTime4->setStyleSheet("");
                }
                ui->labelTime4->setText(timelap);

                // Update player last time
                m_PlayerLastLapTime[3] = curtime;
                break;
            }
        case e_QSlotRacingPlayer5:
            {
                // Check last  lap time
                if (curtime < m_PlayerLastLapTime[4] && (m_PlayerLastLapTime[4] > 0))
                {
                    // Faster lap
                    // Set background color green, text color white
                    ui->labelTime5->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                }
                else if (curtime > m_PlayerLastLapTime[4] && (m_PlayerLastLapTime[4] > 0))
                {
                    // Slower lap
                    // Set background color red, text color white
                    ui->labelTime5->setStyleSheet("background-color: rgb(250, 0, 0);color: rgb(255, 255, 255);");
                }
                else
                {
                    // Same time lap
                    // Remove background color
                    ui->labelTime5->setStyleSheet("");
                }
                ui->labelTime5->setText(timelap);

                // Update player last time
                m_PlayerLastLapTime[4] = curtime;
                break;
            }
        case e_QSlotRacingPlayer6:
            {
                // Check last  lap time
                if ((curtime < m_PlayerLastLapTime[5]) && (m_PlayerLastLapTime[5] > 0))
                {
                    // Faster lap
                    // Set background color green, text color white
                    ui->labelTime6->setStyleSheet("background-color: rgb(42, 171, 13);color: rgb(255, 255, 255);");
                }
                else if (curtime > m_PlayerLastLapTime[5] && (m_PlayerLastLapTime[5] > 0))
                {
                    // Slower lap
                    // Set background color red, text color white
                    ui->labelTime6->setStyleSheet("background-color: rgb(250, 0, 0);color: rgb(255, 255, 255);");
                }
                else
                {
                    // Same time lap
                    // Remove background color
                    ui->labelTime6->setStyleSheet("");
                }
                ui->labelTime6->setText(timelap);

                // Update player last time
                m_PlayerLastLapTime[5] = curtime;
                break;
            }
        default:
            {
                break;
            }
        }
    }
}

bool MainWindow::IsNewCrossing(QSlotRacingPlayer_t player, quint32 crossing)
{
    bool    bRet;

    // Initialization
    bRet = false;

    switch(player)
    {
    case e_QSlotRacingPlayer1:
        {
            if (m_PlayerCrossings[0] != crossing)
            {
                bRet = true;
            }
            break;
        }
    case e_QSlotRacingPlayer2:
        {
            if (m_PlayerCrossings[1] != crossing)
            {
                bRet = true;
            }
            break;
        }
    case e_QSlotRacingPlayer3:
        {
            if (m_PlayerCrossings[2] != crossing)
            {
                bRet = true;
            }
            break;
        }
    case e_QSlotRacingPlayer4:
        {
            if (m_PlayerCrossings[3] != crossing)
            {
                bRet = true;
            }
            break;
        }
    case e_QSlotRacingPlayer5:
        {
            if (m_PlayerCrossings[4] != crossing)
            {
                bRet = true;
            }
            break;
        }
    case e_QSlotRacingPlayer6:
        {
            if (m_PlayerCrossings[5] != crossing)
            {
                bRet = true;
            }
            break;
        }
    default:
        {
            break;
        }
    }

    return bRet;
}

void MainWindow::on_btnQualy_clicked()
{
    UpdateRaceStatus(e_QSlotRacingQualyMode);
}
