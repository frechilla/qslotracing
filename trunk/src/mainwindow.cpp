#include <QtCore/QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"
#include "delegate.h"
#include <iostream>
#include <qpixmap.h>
#include <qbitmap.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 lightgreen, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");

    // Initialize window widgets backgrounds
    InitFuelBackground();

    // Serial port initialization
    OpenSerialPort();

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

    default:
    {
        // highly unexpected
        Q_ASSERT(0);
    }

    } // switch (a_event->EventType())

    //TODO remove
    static ConfigDialog *diag = new ConfigDialog(this);
    diag->exec();
}

void MainWindow::on_BtnConfigure_clicked()
{
    QString    PlayerName;
    bool       PlayerFlag;
    int        PlayerCar;

    // Show configuration dialog
    m_config.exec();

    // Get configuration data
    m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 1);
    qDebug()<<"Player 1:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

    // Configure players
    ConfigurePlayer1(PlayerName, PlayerFlag, PlayerCar);

    m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 2);
    qDebug()<<"Player 2:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

    // Configure players
    ConfigurePlayer2(PlayerName, PlayerFlag, PlayerCar);

    m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 3);
    qDebug()<<"Player 3:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

    // Configure players
    ConfigurePlayer3(PlayerName, PlayerFlag, PlayerCar);

    m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 4);
    qDebug()<<"Player 4:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

    // Configure players
    ConfigurePlayer4(PlayerName, PlayerFlag, PlayerCar);

    m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 5);
    qDebug()<<"Player 5:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

    // Configure players
    ConfigurePlayer5(PlayerName, PlayerFlag, PlayerCar);

    m_config.GetPlayer(PlayerName, PlayerFlag, PlayerCar, 6);
    qDebug()<<"Player 6:"<<PlayerName<<" "<<PlayerFlag<<" "<<PlayerCar;

    // Configure players
    ConfigurePlayer6(PlayerName, PlayerFlag, PlayerCar);
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

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    SetCar1Fuel(value);
    SetCar2Fuel(value);
    SetCar3Fuel(value);
    SetCar4Fuel(value);
    SetCar5Fuel(value);
    SetCar6Fuel(value);
}

void MainWindow::on_pushButton_clicked()
{
    // list of files to be read by the ascii sniffer
    QList<QString> fileList;
    fileList.push_back("C:\\file");
    // building up the ascii sniffer
    SnifferFileAscii asciiSniffer(fileList);
    
    // SCX protocol analyzer and message factory
    SCXProtoAnalyzer scxAnalyzer;
    SCXMsgFactory msgFactory;
    
    // SCXMsgFactory will be processing all bytes sniffed
    // by the asciiSniffer
    asciiSniffer.AddProcessorDelegate(
        MakeDelegate(&SCXMsgFactory::Parse, &msgFactory));

    // all bytes will also be notified into the serial monitor window
    //asciiSniffer.AddProcessorDelegate(MakeDelegate(&SerialMonitor::ReadData, &m_monitor));

    // connect message factory with proto analyzer
    msgFactory.SetMessageProcessorDelegate(
            MakeDelegate(&SCXProtoAnalyzer::ProcessMsg, &scxAnalyzer));

    // connect output events coming from SCXProtoAnalyzer to the window
    scxAnalyzer.SetEventProcessorDelegate(
            MakeDelegate(&MainWindow::ProcessEvent, this));

    // start the testing. stuff will be printed on the screen!!
    asciiSniffer.Start();

    std::cout << "Messages discarded: "
              << msgFactory.GetBytesDiscardedCount() << std::endl;
}

void MainWindow::on_pushButton_2_clicked()
{
     ui->EditLaps1->setText("2/100");
}

void MainWindow::OpenSerialPort()
{
    // set settings to open the serial port
    m_serial.SetDeviceName("COM13");
    m_serial.SetBaudRate(AbstractSerial::BaudRate115200);
    m_serial.SetDataBits(AbstractSerial::DataBits8);
    m_serial.SetParity(AbstractSerial::ParityNone);
    m_serial.SetStopBits(AbstractSerial::StopBits1);
    m_serial.SetFlowControl(AbstractSerial::FlowControlOff);
    
    // Open it!
    m_serial.OpenSerial();
    SetMainWindowDelegate();
}

void MainWindow::on_pushButton_3_clicked()
{
    m_serial.Write();
}

void MainWindow::slotRead(const quint8* a_buffer, quint32 a_bufferSize)
{
    qDebug() << "MainWindow::Readed is : " << a_bufferSize << " bytes:";
    for (quint32 i = 0; i < a_bufferSize; i++)
    {
        qDebug() << a_buffer[i];
    }
}

void MainWindow::on_serial_monitor_clicked()
{
    //static SerialMonitor *diag = new SerialMonitor(this);
    m_monitor.show();
}

void MainWindow::SetMainWindowDelegate()
{
    m_serial.AddProcessorDelegate(MakeDelegate(&MainWindow::slotRead, this));
    m_serial.AddProcessorDelegate(MakeDelegate(&SerialMonitor::ReadData, &m_monitor));
}

void MainWindow::ConfigurePlayer1(QString player, bool flag, int car)
{
    qDebug() <<"Configuring player 1: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps1->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos1->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos1->setText("3");

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
    qDebug() <<"Configuring player 2: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps2->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos2->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos2->setText("3");

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
    qDebug() <<"Configuring player 3: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps3->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos3->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos3->setText("3");

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
    qDebug() <<"Configuring player 4: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps4->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos4->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos4->setText("3");

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
    qDebug() <<"Configuring player 5: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps5->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos5->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos5->setText("3");

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
    qDebug() <<"Configuring player 6: "<<player<<" "<<flag<<" "<<car;

    // Configure player
    if (flag == true)
    {
        ui->editLaps6->setStyleSheet("background-color: rgb(170, 255, 255);");
        ui->editPos6->setStyleSheet("background-color: rgb(255, 255, 255);");

        // Set default position
        ui->editPos6->setText("3");

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

void MainWindow::on_btnTestEventFuel_clicked()
{

}
