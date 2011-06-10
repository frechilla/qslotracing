#include <QtCore/QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_serialSniffer(parent),
    m_asciiSniffer(parent),
    m_msgFactory(parent),
    m_scxAnalyzer(parent)
{
    ui->setupUi(this);
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 lightgreen, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");

    // Initialize protocol stacks (basically connect slots to signals)
    InitializeProtoStack();

    // Initialize window widgets backgrounds
    InitFuelBackground();

    // Default configuration
    ConfigurePlayer1("", 0, -1);
    ConfigurePlayer2("", 0, -1);
    ConfigurePlayer3("", 0, -1);
    ConfigurePlayer4("", 0, -1);
    ConfigurePlayer5("", 0, -1);
    ConfigurePlayer6("", 0, -1);

}

MainWindow::~MainWindow()
{
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
                       SIGNAL(bytesRead(QByteArray)),
                       SLOT(Parse(QByteArray)));

    m_msgFactory.connect(&m_asciiSniffer,
                       SIGNAL(bytesRead(QByteArray)),
                       SLOT(Parse(QByteArray)));


    // all sniffed bytes will also be notified into the serial monitor window
    // again, for both serial and ascii sniffers
    m_monitor.connect(&m_serialSniffer,
                      SIGNAL(bytesRead(QByteArray)),
                      SLOT(ReadData(QByteArray)));

    m_monitor.connect(&m_asciiSniffer,
                      SIGNAL(bytesRead(QByteArray)),
                      SLOT(ReadData(QByteArray)));


    // Message factory output will be sent into the proto analyzer
    m_scxAnalyzer.connect(&m_msgFactory,
                        SIGNAL(MsgParsed(QSharedPointer<QSlotRacingMsg>)),
                        SLOT(ProcessMsg(QSharedPointer<QSlotRacingMsg>)));


    // Events coming from the SCXProtoAnalyzer will be processed by this window
    this->connect(&m_scxAnalyzer,
                  SIGNAL(ProtoEvent(QSharedPointer<QSlotRacingEvent>)),
                  SLOT(ProcessEvent(QSharedPointer<QSlotRacingEvent>)));


    //TODO DEBUG code
    // serial sniffer is also connected to MainWindow::slotRead
    // it should be deleted at some point
    this->connect(&m_serialSniffer,
                  SIGNAL(bytesRead(QByteArray)),
                  SLOT(slotRead(QByteArray)));
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
        quint8 speed = 0;

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer1, lights, lane_change, speed);
        //qDebug()<<" Controller 1: ligths("<<lights<<") lane change("<<lane_change<<") speed ("<<speed<<")";
        SetController1(lights, lane_change, speed);

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer2, lights, lane_change, speed);
        //qDebug()<<" Controller 2: ligths("<<lights<<") lane change("<<lane_change<<") speed ("<<speed<<")";
        SetController2(lights, lane_change, speed);

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer3, lights, lane_change, speed);
        //qDebug()<<" Controller 3: ligths("<<lights<<") lane change("<<lane_change<<") speed ("<<speed<<")";
        SetController3(lights, lane_change, speed);

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer4, lights, lane_change, speed);
        //qDebug()<<" Controller 4: ligths("<<lights<<") lane change("<<lane_change<<") speed ("<<speed<<")";
        SetController4(lights, lane_change, speed);

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer5, lights, lane_change, speed);
        //qDebug()<<" Controller 5: ligths("<<lights<<") lane change("<<lane_change<<") speed ("<<speed<<")";
        SetController5(lights, lane_change, speed);

        retValue = controllerEvent->GetPlayersControllerData(e_QSlotRacingPlayer6, lights, lane_change, speed);
        //qDebug()<<" Controller 6: ligths("<<lights<<") lane change("<<lane_change<<") speed ("<<speed<<")";
        SetController6(lights, lane_change, speed);
        break;
    } // case e_QSlotRacingEvent_Controller
    case e_QSlotRacingEvent_Ranking:
    {
        QSharedPointer<QSlotRacingEventRanking> rankingEvent =
                a_event.staticCast<QSlotRacingEventRanking>();

        //quint8 carId;
        quint8 pos;
        quint8 retValue;
        bool   moreThan15;
        bool   carFlag;
        quint8 lapsBehind;

        // Get data from position 1
        retValue = rankingEvent->GetRankingDataByCar(1, pos, moreThan15, carFlag, lapsBehind);
        //qDebug()<<"CAR 1: "<<pos<<" "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind;
        //retValue = rankingEvent->GetRankingDataByPos(1, moreThan15, carFlag, lapsBehind, carId);
        //qDebug()<<"Pos 1: "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind<<" "<<carId;

        // Update position from retrieved carId
        UpdateCarPosition(1, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 2
        retValue = rankingEvent->GetRankingDataByCar(2, pos, moreThan15, carFlag, lapsBehind);
        //qDebug()<<"CAR 2: "<<pos<<" "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind;
        //retValue = rankingEvent->GetRankingDataByPos(2, moreThan15, carFlag, lapsBehind, carId);
        //qDebug()<<"Pos 2: "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind<<" "<<carId;

        // Update position from retrieved carId
        UpdateCarPosition(2, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 3
        retValue = rankingEvent->GetRankingDataByCar(3, pos, moreThan15, carFlag, lapsBehind);
        //qDebug()<<"CAR 3: "<<pos<<" "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind;
        //retValue = rankingEvent->GetRankingDataByPos(3, moreThan15, carFlag, lapsBehind, carId);
        //qDebug()<<"Pos 3: "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind<<" "<<carId;

        // Update position from retrieved carId
        UpdateCarPosition(3, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 4
        retValue = rankingEvent->GetRankingDataByCar(4, pos, moreThan15, carFlag, lapsBehind);
        //qDebug()<<"CAR 4: "<<pos<<" "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind;
        //retValue = rankingEvent->GetRankingDataByPos(4, moreThan15, carFlag, lapsBehind, carId);
        //qDebug()<<"Pos 4: "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind<<" "<<carId;

        // Update position from retrieved carId
        UpdateCarPosition(4, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 5
        retValue = rankingEvent->GetRankingDataByCar(5, pos, moreThan15, carFlag, lapsBehind);
        //qDebug()<<"CAR 5: "<<pos<<" "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind;
        //retValue = rankingEvent->GetRankingDataByPos(5, moreThan15, carFlag, lapsBehind, carId);
        //qDebug()<<"Pos 5: "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind<<" "<<carId;

        // Update position from retrieved carId
        UpdateCarPosition(5, pos, carFlag, moreThan15, lapsBehind);

        // Get data from position 6
        retValue = rankingEvent->GetRankingDataByCar(6, pos, moreThan15, carFlag, lapsBehind);
        //qDebug()<<"CAR 6: "<<pos<<" "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind;
        //retValue = rankingEvent->GetRankingDataByPos(6, moreThan15, carFlag, lapsBehind, carId);
        //qDebug()<<"Pos 6: "<<moreThan15<<" "<<carFlag<<" "<<lapsBehind<<" "<<carId;

        // Update position from retrieved carId
        UpdateCarPosition(6, pos, carFlag, moreThan15, lapsBehind);

        break;
    } // case e_QSlotRacingEvent_Ranking
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

    // start the testing. a byte will be read each 25ms
    m_asciiSniffer.Start(25);

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
    
    // Open it!
    m_serialSniffer.OpenSerial();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_serialSniffer.Write();
}

void MainWindow::slotRead(QByteArray ba)
{
    qDebug() << "MainWindow::slotRead is : " << ba.size() << " bytes:";
    for (qint32 i = 0; i < ba.size(); i++)
    {
        qDebug() << "MainWindow::slotRead: " << ba.at(i);
    }
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
    char       posStr[5];

    memset(posStr, 0, 5);

    //qDebug()<<"Update pos: (carId)"<<carId<<" (pos)"<<pos<<" (carFlag)"<<carFlag<<" (moreThan15)"<<moreThan15<<" (lapsBehind)"<<lapsBehind;
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
                sprintf(posStr, "%d", pos);
                data = QString::fromLocal8Bit(posStr);
                ui->editPos1->setText(data);
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
