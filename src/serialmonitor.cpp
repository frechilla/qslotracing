#include "serialmonitor.h"
#include "ui_serialmonitor.h"
#include <QtCore/QDebug>

SerialMonitor::SerialMonitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialMonitor)
{
    ui->setupUi(this);

    flag_show = 0;
}

SerialMonitor::~SerialMonitor()
{
    delete ui;

    if (m_file != 0)
    {
        if (m_file->isOpen())
        {
            m_file->close();
        }

        delete m_file;

        qDebug() << "cerrar fichero bien";
    }
}

void SerialMonitor::on_start_capture_clicked()
{
    flag_show = 1;
    ui->data_packets->insertPlainText("AAFF555555\n");
}

void SerialMonitor::on_stop_capture_clicked()
{

}

void SerialMonitor::AddTrace()
{
    if (flag_show == 1)
    {
        //ui->data_packets->insertPlainText("AA");
    }
}

void SerialMonitor::on_save_clicked()
{
    QString texto;

    // Open the file
    m_file = new QFile("serial.dat");

    // Check file
    if (m_file->open(QIODevice::WriteOnly) == true)
    {
        qDebug()<<"fichero ok";

        texto = ui->data_packets->toPlainText();

        qDebug() << qPrintable(texto);


        m_file->write(qPrintable(texto));
    }
    else
    {
        qDebug()<<"fichero mal";
    }
}
