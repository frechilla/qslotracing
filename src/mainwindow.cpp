#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"

#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"
#include "delegate.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 lightgreen, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");

    // Initialize window widgets backgrounds
    InitFuelBackground();

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
    static ConfigDialog *diag = new ConfigDialog(this);
    diag->exec();
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
    asciiSniffer.SetProcessorDelegate(
        MakeDelegate(&SCXMsgFactory::Parse, &msgFactory));

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
