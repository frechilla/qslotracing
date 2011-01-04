#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 lightgreen, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");
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
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer2);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer3);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer4);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer5);
        value = fuelEvent->GetPlayersFuel(e_QSlotRacingPlayer6);

        break;
    } // case e_QSlotRacingEvent_Fuel

    default:
    {
        // highly unexpected
        Q_ASSERT(0);
    }

    } // switch (a_event->EventType())
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_boton1_clicked()
{
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 lightgreen, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");
   ui->Fuel1->setValue(6);
}

void MainWindow::on_pushButton_3_clicked()
{
       setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 yellow, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");
    ui->Fuel1->setValue(3);
}

void MainWindow::on_pushButton_2_clicked()
{
    setStyleSheet("QProgressBar:horizontal {border: 1px solid gray;border-radius: 3px;background: black;padding: 1px;text-align: right;margin-right: 10ex; }QProgressBar::chunk:horizontal {background: qlineargradient(x1: 0, y1: 0.5, x2: 3, y2: 0.5, stop: 0 red, stop: 1 black); margin-right: 2px; /* space */ width: 10px; }");
 ui->Fuel1->setValue(1);

}

void MainWindow::on_BtnConfigure_clicked()
{
    static ConfigDialog *diag = new ConfigDialog(this);
    diag->exec();
}

