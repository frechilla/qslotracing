#ifndef SNIFFERFILEASCII_H
#define SNIFFERFILEASCII_H

#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>

/// @brief class which porpuse is to emulate a serial interface
/// it reads bytes encoded in ascii from a file and encodes them into binary
/// its output is a buffer of data bytes
class SnifferFileAscii : public QObject
{
    Q_OBJECT
public:

    /// @brief constructor
    /// @param list of filenames to be read by this class
    /// that list of files will be read one by one inside the Start function
    explicit SnifferFileAscii(
            const QList<QString> &a_filenameList,
            QObject *parent = 0);
    virtual ~SnifferFileAscii();

    /// @brief start parsing the data files requested
    /// It won't return until all files have been parsed
    void Start();

private:
    /// @brief list of filenames
    QList<QString> m_filenameList;

    /// @brief latest ascii char processed by ProcessChar
    /// it is set to '\0' when the latest character is not [A-F0-9] or 
    /// when the latest character forced this object to notify a raw byte
    char m_latestAsciiProcessed;

    /// @brief parses an ascii character.
    /// it might detect a raw byte described in the ascii input. If that is the
    /// case it will "notify" it to the listener.
    /// it only processes the following ascii characters: [A-F0-9] so character
    /// should be converted into uppercase before calling to this function
    /// any other ascii character is ignored. If a non [A-F0-9] character is 
    /// found it will be understood as a separator, so the following string:
    ///     FF F FF
    /// would generate 255 15 255 (FF 0F FF)
    /// @param An ascii character that might represent a part of a raw byte
    ///        For instance, you need two ascii bytes to form a raw byte:
    ///        FF-> 255
    ///        F0-> 240
    void ProcessChar(char a_asciiCharacter);

    /// @brief transforms an ascii character into its corresponding binary value
    /// @param an ascii chracter. It MUST satisfy the following regex 
    ///        [A-Fa-f0-9]
    /// @return corresponding binary value to the 'a_asciiCharacter' parameter
    quint8 asciiToRaw(char a_asciiCharacter);



signals:
    void bytesRead(QByteArray ba);

private slots:
    /// @brief prints the buffer received as parameter
    /// it can be used as a dummy slot to print the sniffed stuff
    /// @param data buffer
    void PrintBuffer(QByteArray);
};

#endif // SNIFFERFILEASCII_H
