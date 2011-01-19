#ifndef SNIFFERFILEASCII_H
#define SNIFFERFILEASCII_H

#include <QString>
#include <QList>
#include "delegate.h" // delegate of the upper layer that will process
                      // bytes sniffed by this class

/// @brief class which porpuse is to emulate a serial interface
/// it reads bytes encoded in ascii from a file and encodes them into binary
/// its output is a buffer of data bytes
class SnifferFileAscii
{
public:
    /// @brief type of the delegate where bytes sniffed are sent
    typedef Delegate< void(const quint8*, quint32) > QSnifferDelegate_t;

    /// @brief constructor
    /// @param list of filenames to be read by this class
    /// that list of files will be read one by one inside the Start function
    SnifferFileAscii(
            const QList<QString> &a_filenameList);
    virtual ~SnifferFileAscii();

    /// @brief start parsing the data files requested
    /// It won't return until all files have been parsed
    void Start();

    /// @brief sets the delegate in charge of processing bytes sniffed by this class
    void SetProcessorDelegate(QSnifferDelegate_t a_delegate);

private:
    /// @brief list of filenames
    QList<QString> m_filenameList;

    /// @brief latest ascii char processed by ProcessChar
    /// it is set to '\0' when the latest character is not [A-F0-9] or 
    /// when the latest character forced this object to notify a raw byte
    char m_latestAsciiProcessed;

    /// @brief function delegate where new messages will be sent
    QSnifferDelegate_t m_byteDelegate;

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

    // prevent this class from being constructed without the proper arguments
    SnifferFileAscii();
    SnifferFileAscii(const SnifferFileAscii &a_src);
    SnifferFileAscii& operator=(const SnifferFileAscii &a_src);
};

#endif // SNIFFERFILEASCII_H
