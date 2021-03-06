#include <QtCore/QDebug>

#include "configdialog.h"
#include "ui_configdialog.h"
#include <qbitmap.h>
#include <qpainter.h>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    // Initialize configured flag
    m_configured = false;
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_buttonBox_accepted()
{
    m_configured = true;
    m_port = ui->ListComms->currentText();
}

void ConfigDialog::GetPlayer(QString &player, bool &flag, int &car, int PlayerId)
{
    switch (PlayerId)
    {
    case 1:
        {
            player = ui->EditName1->text();
            flag = ui->checkPlayer1->checkState();
            car = ui->ListCar1->currentIndex();
            break;
        }
    case 2:
        {
            player = ui->EditName2->text();
            flag = ui->checkPlayer2->checkState();
            car = ui->ListCar2->currentIndex();
            break;
        }
    case 3:
        {
            player = ui->EditName3->text();
            flag = ui->checkPlayer3->checkState();
            car = ui->ListCar3->currentIndex();
            break;
        }
    case 4:
        {
            player = ui->EditName4->text();
            flag = ui->checkPlayer4->checkState();
            car = ui->ListCar4->currentIndex();
            break;
        }
    case 5:
        {
            player = ui->EditName5->text();
            flag = ui->checkPlayer5->checkState();
            car = ui->ListCar5->currentIndex();
            break;
        }
    case 6:
        {
            player = ui->EditName6->text();
            flag = ui->checkPlayer6->checkState();
            car = ui->ListCar6->currentIndex();
            break;
        }
    }
;
}

void ConfigDialog::GetConfigured(bool &configured)
{
    configured = m_configured;
}

void ConfigDialog::GetSerialPort(QString &port)
{
    port = m_port;
}


void ConfigDialog::on_EditName1_textEdited(QString )
{
    ui->checkPlayer1->setChecked(true);
}

void ConfigDialog::on_EditName2_textEdited(QString )
{
    ui->checkPlayer2->setChecked(true);
}

void ConfigDialog::on_EditName3_textEdited(QString )
{
    ui->checkPlayer3->setChecked(true);
}

void ConfigDialog::on_EditName4_textEdited(QString )
{
    ui->checkPlayer4->setChecked(true);
}

void ConfigDialog::on_EditName5_textEdited(QString )
{
    ui->checkPlayer5->setChecked(true);
}

void ConfigDialog::on_EditName6_textEdited(QString )
{
    ui->checkPlayer6->setChecked(true);
}

void ConfigDialog::on_EditName1_textChanged(QString str)
{
    if (str == "")
    {
        ui->checkPlayer1->setChecked(false);
    }
}

void ConfigDialog::on_EditName2_textChanged(QString str)
{
    if (str == "")
    {
        ui->checkPlayer2->setChecked(false);
    }
}

void ConfigDialog::on_EditName3_textChanged(QString str)
{
    if (str == "")
    {
        ui->checkPlayer3->setChecked(false);
    }
}

void ConfigDialog::on_EditName4_textChanged(QString str)
{
    if (str == "")
    {
        ui->checkPlayer4->setChecked(false);
    }
}

void ConfigDialog::on_EditName5_textChanged(QString str)
{
    if (str == "")
    {
        ui->checkPlayer5->setChecked(false);
    }
}

void ConfigDialog::on_EditName6_textChanged(QString str)
{
    if (str == "")
    {
        ui->checkPlayer6->setChecked(false);
    }
}
