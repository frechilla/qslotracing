#include "recorderbinary.h"
#include <QtCore/QDebug>
#include <QDateTime>

RecorderBinary::RecorderBinary(const QString &a_filenamePrefix,
                               quint32 a_rotationSize):
        m_filenamePrefix(a_filenamePrefix),
        m_rotationSize(a_rotationSize),
        m_currentFileSize(0),
        m_file(0)
{
    // create the 1st dump file on construction time
    RotateDumpFile();
}

RecorderBinary::~RecorderBinary()
{
    if (m_file != 0)
    {
        if (m_file->isOpen())
        {
            m_file->close();
        }

        delete m_file;
    }
}

void RecorderBinary::Dump(const quint8* a_dataBuffer, quint32 a_bufferSize)
{
    if ((a_bufferSize + m_currentFileSize) < m_rotationSize)
    {
        // just dump the buffer into the file. It still fits
        m_dataStream.writeRawData(
                reinterpret_cast<const char*>(a_dataBuffer),
                a_bufferSize);

        // update current file size
        m_currentFileSize += a_bufferSize;
    }
    else
    {
        while ( ((a_bufferSize + m_currentFileSize) >= m_rotationSize) &&
                (a_bufferSize > 0) )
        {
            // dump 1st all data that fits in current dump file
            m_dataStream.writeRawData(
                    reinterpret_cast<const char*>(a_dataBuffer),
                    m_rotationSize - m_currentFileSize);

            // update the number of bytes left and the pointer to
            // the start of the data buffer
            a_bufferSize -= m_rotationSize - m_currentFileSize;
            a_dataBuffer += m_rotationSize - m_currentFileSize;

            // rotate the dumping file
            RotateDumpFile();

            // update current file size to 0. A new file has just been created
            m_currentFileSize = 0;
        }

        // dump the rest of the info in it
        if (a_bufferSize > 0)
        {
            // just dump the buffer into the file. It still fits
            m_dataStream.writeRawData(
                    reinterpret_cast<const char*>(a_dataBuffer),
                    a_bufferSize);

            // update current file size
            m_currentFileSize += a_bufferSize;
        }
    }
}

void RecorderBinary::RotateDumpFile()
{
    if (m_file != 0)
    {
        if (m_file->isOpen())
        {
            m_file->close();
        }

        delete m_file;
    }
    
    // calculate new file name. This is all a bit weird, based on the 'arg' function
    //     QString arg ( int a, int fieldWidth = 0, int base = 10, const QChar & fillChar = QLatin1Char( ' ' ) ) const
    QDateTime dateTime = QDateTime::currentDateTime();
    QString filename;
    filename = QString("%1%2%3%4%5%6%7%8")        
                .arg(m_filenamePrefix)
                .arg(dateTime.date().year(),        4, 10, QChar('0'))
                .arg(dateTime.date().month(),       2, 10, QChar('0'))
                .arg(dateTime.date().day(),         2, 10, QChar('0'))
                .arg(dateTime.time().hour(),        2, 10, QChar('0'))
                .arg(dateTime.time().minute(),      2, 10, QChar('0'))
                .arg(dateTime.time().second(),      2, 10, QChar('0'))
                .arg(dateTime.time().msec(),        3, 10, QChar('0'));

    //TODO error handling
    m_file = new QFile(filename);
    if (m_file->open(QIODevice::WriteOnly) == true)
    {
        m_dataStream.setDevice(m_file);
    }
    else
    {
        // unset the file where data will be dumped
        m_dataStream.setDevice(0);

        qDebug() << "Could not open file \""
                 << filename
                 << "\" for writing. Binary recorder disabled";
    }
}
