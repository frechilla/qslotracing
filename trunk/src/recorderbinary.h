#ifndef RECORDERBINARY_H
#define RECORDERBINARY_H

#include <QString>
#include <QFile>
#include <QDataStream>

// files will be rotated when they reach up to 1 Mb by default
#define BINARY_DEFAULT_ROTATION_SIZE 1048576

/// @brief class which porpuse is to dump every byte into file
/// it writes every byte into a file specified by the user of the class
/// It is called "binary" 'cos it writes down the info into the file "raw"
class RecorderBinary
{
public:
    /// @param files with the recorded data will be named 'a_fileNamePrefix'YYYYMMDDHHmmSSlll
    ///        This prefix may also contain the path to wherever the destination folder is
    ///        YYYY: Year, as in 2010
    ///        MM: Month number, january is 01, december 12
    ///        DD: Day of the month. From 01 to 31
    ///        HH: Hour of day in 24h format. From 00 to 23
    ///        mm: Minute in hour. From 00 to 59
    ///        SS: Second. From 00 to 59
    ///        lll: Milliseconds in this second, from 000 to 999
    /// @param dump file will be rotate every 'a_rotationSize' bytes
    RecorderBinary(const QString &a_filenamePrefix,
                   quint32 a_rotationSize = BINARY_DEFAULT_ROTATION_SIZE);
    virtual ~RecorderBinary();

    /// @brief dumps a_dataBuffer into the current file
    /// if it reaches to the maximum size the dumping file will be rotated
    /// @param pointer to the data
    /// @param size of the buffer
    void Dump(const quint8* a_dataBuffer, quint32 a_bufferSize);

private:
    /// prefix that all dump filenames will contain
    QString m_filenamePrefix;

    /// files with the raw data will be rotated when they reach up to this size
    quint32 m_rotationSize;

    /// current size, in bytes, of the dumping file
    quint32 m_currentFileSize;

    /// pointer to the file currently used to dump data into it
    QFile* m_file;

    /// binary stream where data is dumped
    QDataStream m_dataStream;

    /// @brief closes current file and creates the new one
    void RotateDumpFile();
};

#endif // RECORDERBINARY_H
