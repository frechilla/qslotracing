#include <QtCore/QDebug>

#include "controllerdlg.h"
#include "ui_controllerdlg.h"

ControllerDlg::ControllerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControllerDlg)
{
    ui->setupUi(this);

    // Initialize controller displays
    ui->gas1->setValue(0);
    ui->gas2->setValue(0);
    ui->gas3->setValue(0);
    ui->gas4->setValue(0);
    ui->gas5->setValue(0);
    ui->gas6->setValue(0);

    // Initialize change button states
    ui->Change1->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->Change2->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->Change3->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->Change4->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->Change5->setStyleSheet("background-color: rgb(0, 0, 255);");
    ui->Change6->setStyleSheet("background-color: rgb(0, 0, 255);");
}

ControllerDlg::~ControllerDlg()
{
    delete ui;
}

void ControllerDlg::on_horizontalSlider_valueChanged(int value)
{
    SetGas1(value);
    SetGas2(value);
    SetGas3(value);
    SetGas4(value);
    SetGas5(value);
    SetGas6(value);
}

void ControllerDlg::SetGas1(int value)
{
    if (this->isVisible() == true)
    {
        ui->gas1->setValue(value);
    }
}

void ControllerDlg::SetGas2(int value)
{
    if (this->isVisible() == true)
    {
        ui->gas2->setValue(value);
    }
}

void ControllerDlg::SetGas3(int value)
{
    if (this->isVisible() == true)
    {
        ui->gas3->setValue(value);
    }
}

void ControllerDlg::SetGas4(int value)
{
    if (this->isVisible() == true)
    {
        ui->gas4->setValue(value);
    }
}

void ControllerDlg::SetGas5(int value)
{
    if (this->isVisible() == true)
    {
        ui->gas5->setValue(value);
    }
}

void ControllerDlg::SetGas6(int value)
{
    if (this->isVisible() == true)
    {
        ui->gas6->setValue(value);
    }
}


void ControllerDlg::on_btnchange_clicked()
{
    SetChange1(false);
    SetChange2(false);
    SetChange3(false);
    SetChange4(false);
    SetChange5(false);
    SetChange6(false);
}

void ControllerDlg::on_pushButton_clicked()
{
    SetChange1(true);
    SetChange2(true);
    SetChange3(true);
    SetChange4(true);
    SetChange5(true);
    SetChange6(true);
}

void ControllerDlg::SetChange1(bool change)
{
    if (this->isVisible() == true)
    {
        if (change == true)
        {
            ui->Change1->setStyleSheet("background-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->Change1->setStyleSheet("background-color: rgb(0, 0, 255);");
        }
    }
}

void ControllerDlg::SetChange2(bool change)
{
    if (this->isVisible() == true)
    {
        if (change == true)
        {
            ui->Change2->setStyleSheet("background-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->Change2->setStyleSheet("background-color: rgb(0, 0, 255);");
        }
    }
}

void ControllerDlg::SetChange3(bool change)
{
    if (this->isVisible() == true)
    {
        if (change == true)
        {
            ui->Change3->setStyleSheet("background-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->Change3->setStyleSheet("background-color: rgb(0, 0, 255);");
        }
    }
}

void ControllerDlg::SetChange4(bool change)
{
    if (this->isVisible() == true)
    {
        if (change == true)
        {
            ui->Change4->setStyleSheet("background-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->Change4->setStyleSheet("background-color: rgb(0, 0, 255);");
        }
    }
}

void ControllerDlg::SetChange5(bool change)
{
    if (this->isVisible() == true)
    {
        if (change == true)
        {
            ui->Change5->setStyleSheet("background-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->Change5->setStyleSheet("background-color: rgb(0, 0, 255);");
        }
    }
}

void ControllerDlg::SetChange6(bool change)
{
    if (this->isVisible() == true)
    {
        if (change == true)
        {
            ui->Change6->setStyleSheet("background-color: rgb(0, 255, 0);");
        }
        else
        {
            ui->Change6->setStyleSheet("background-color: rgb(0, 0, 255);");
        }
    }
}
