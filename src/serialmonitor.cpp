#include "serialmonitor.h"
#include "ui_serialmonitor.h"
#include <QtCore/QDebug>

#define BYTES_PER_LINE 16

SerialMonitor::SerialMonitor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialMonitor)
{
    ui->setupUi(this);
    m_file = NULL;

    flag_show = 0;
}

SerialMonitor::~SerialMonitor()
{
    if (m_file != 0)
    {
        if (m_file->isOpen())
        {
            m_file->close();
        }

        delete m_file;

        //qDebug() << "cerrar fichero bien";
    }
    else
    {
        //qDebug() << "cerrando SerialMonitor...";
    }

    delete ui;
}

void SerialMonitor::on_start_capture_clicked()
{
    flag_show = 1;
    ui->data_packets->insertPlainText("Starting capture...\n");
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
        texto = ui->data_packets->toPlainText();

        qDebug() << qPrintable(texto);

        m_file->write(qPrintable(texto));
    }
    else
    {
        //qDebug()<<"fichero mal";
    }
}

void SerialMonitor::ReadData(QByteArray a_buffer)
{
    QString       strData;
    static char   data[5] = "";
    static qint32 bytesThisLine = 0;

    if (flag_show == 1)
    {
        strData = "";
        for (qint32 i = 0; i < a_buffer.count(); i++)
        {
            sprintf(data, "%02X", static_cast<quint8>(a_buffer.at(i)));
            strData = QString::fromLocal8Bit(data);
            ui->data_packets->insertPlainText(strData + QString(" "));

            bytesThisLine++;
            if (bytesThisLine == BYTES_PER_LINE)
            {
                bytesThisLine = 0;
                ui->data_packets->insertPlainText("\n");
            }
        }
    }
}
