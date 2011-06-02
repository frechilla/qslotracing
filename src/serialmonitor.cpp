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

void SerialMonitor::ReadData(const quint8* a_buffer, quint32 a_bufferSize)
{
    QString data;

    qDebug() << "SerialMonitor::Readed is : " << a_bufferSize << " bytes:";
    for (quint32 i = 0; i < a_bufferSize; i++)
    {
        qDebug() << a_buffer[i];
    }

    if (flag_show == 1)
    {
        for (quint32 i = 0; i < a_bufferSize; i++)
        {
            qDebug() << a_buffer[i];
        }
        //ui->data_packets->insertPlainText("AA");
    }
}
