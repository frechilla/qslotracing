#ifndef SNIFFERFILEASCII_H
#define SNIFFERFILEASCII_H

#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QTimer>

/// @brief class which porpuse is to emulate a serial interface
/// it reads bytes encoded in ascii from a file and encodes them into binary
/// its output is a buffer of data bytes
class SnifferFileAscii : public QObject
{
    Q_OBJECT
public:

    /// @brief constructor
    explicit SnifferFileAscii(QObject *parent = 0);
    virtual ~SnifferFileAscii();

    /// @brief add a file to the list of ascii that will be parsed
    /// WARNING: Parsing process will start from the beginning if a new file is added
    void AddAsciiFile(const QString& a_filename);

    /// @brief starts (or restarts) the parsing of the data files requested
    /// It sets up a timer to which will be used to "sleep"
    /// between each one of the bytes read from file
    /// WARNING: Each time this function is called the parsing process will start
    /// from thje beginning. If the timer was already started the parsing process
    /// will be stopped and restarted
    /// @param amount of milliseconds to sleep between each two consecutive parsed bytes
    void Start(int msec);

private:    
    /// @brief timer to trigger the parsing of the files
    QTimer m_timer;

    /// @brief list of filenames
    QList<QString> m_filenameList;

    /// @brief filename of m_currentOpenedFile
    QList<QString>::const_iterator m_itFilename;

    /// @brief currently opened file
    QFile* m_currentOpenedFile;

    /// @brief stream to the file which is currently being parsed
    QTextStream* m_openedFileStream;

    /// @brief latest ascii char processed by ProcessChar
    /// it is set to '\0' when the latest character is not [A-F0-9] or 
    /// when the latest character forced this object to notify a raw byte
    char m_latestAsciiProcessed;

    /// @brief set to true when the ascii file finds a '#' character
    bool m_commentedInput;

    /// restart all internal pointers and variables to ensure the parsing
    /// process starts off the beginning of the files again
    void ResetParsing();

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
    /// @return true if a new bytesRead signal was issued. False otherwise
    bool ProcessChar(char a_asciiCharacter);

    /// @brief transforms an ascii character into its corresponding binary value
    /// @param an ascii chracter. It MUST satisfy the following regex 
    ///        [A-Fa-f0-9]
    /// @return corresponding binary value to the 'a_asciiCharacter' parameter
    quint8 asciiToRaw(char a_asciiCharacter);


signals:
    void bytesRead(QByteArray ba);

private slots:
    /// @brief reads the next byte off the file and issues a bytesRead signal
    /// the bytesRead signal will contain a QByteArray of just 1 byte
    void ReadByteFromFile(void);

    /// @brief prints the buffer received as parameter
    /// it can be used as a dummy slot to print the sniffed stuff
    /// @param data buffer
    void PrintBuffer(QByteArray);
};

#endif // SNIFFERFILEASCII_H
