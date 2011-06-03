#include <QTextStream>
#include <QFile>
#include "snifferfileascii.h"

#define LATEST_ASCII_UNSET '\0'

SnifferFileAscii::SnifferFileAscii(
        const QList<QString> &a_filenameList):
    m_filenameList(a_filenameList),
    m_latestAsciiProcessed(LATEST_ASCII_UNSET)
{
}

SnifferFileAscii::~SnifferFileAscii()
{
}

void SnifferFileAscii::AddProcessorDelegate(QSnifferDelegate_t a_delegate)
{
    m_byteDelegateList.push_back(a_delegate);
}

void SnifferFileAscii::Start()
{
    QChar thisCharacter; // tmp character to parse the ascii file

    QList<QString>::const_iterator filenameIterator;
    for (filenameIterator  = m_filenameList.begin();
         filenameIterator != m_filenameList.end();
         filenameIterator++)
    {
        QFile thisQFile(*filenameIterator);
        if (!thisQFile.open(QIODevice::ReadOnly))
        {
            // could not open current file
            continue;
        }

        QTextStream textInput(&thisQFile);
        // support for unicode files is not needed here, but if it was
        // it would be worth looking at setAutoDetectUnicode
        
        bool commentedInput = false;
        while(textInput.status() == QTextStream::Ok)
        {
            // will exit when the end of the file is reached or there is an
            // error reading from the buffer
            textInput >> thisCharacter;

            if (commentedInput == false)
            {
                ProcessChar(thisCharacter.toUpper().toAscii());

                if (thisCharacter.toAscii() == '#')
                {
                    // everything else in this line must be discarded
                    commentedInput = true;
                }
            }
            else if ((thisCharacter.toAscii() == '\r') ||
                     (thisCharacter.toAscii() == '\n') )
            {
                // end of line reached. Resume processing input
                commentedInput = false;
            }
        } 
    }
}

void SnifferFileAscii::ProcessChar(char a_asciiCharacter)
{
    switch(a_asciiCharacter)
    {
    case '0': // let it fall down
    case '1': // let it fall down
    case '2': // let it fall down
    case '3': // let it fall down
    case '4': // let it fall down
    case '5': // let it fall down
    case '6': // let it fall down
    case '7': // let it fall down
    case '8': // let it fall down
    case '9': // let it fall down
    case 'A': // let it fall down
    case 'B': // let it fall down
    case 'C': // let it fall down
    case 'D': // let it fall down
    case 'E': // let it fall down
    case 'F': // let it fall down
    {
        // expected character
        if (m_latestAsciiProcessed == LATEST_ASCII_UNSET)
        {
            // just save it until we get the next byte
            m_latestAsciiProcessed = a_asciiCharacter;
        }
        else
        {
            quint8 valueFound = asciiToRaw(m_latestAsciiProcessed) << 4;
            valueFound |= asciiToRaw(a_asciiCharacter);
            
            m_latestAsciiProcessed = LATEST_ASCII_UNSET;

            // notify that value we've just found to the upper layers
            NotifyDelegates(&valueFound, 1);
        }

        break;
    }

    default:
    {
        // unexpected character
        if (m_latestAsciiProcessed != LATEST_ASCII_UNSET) 
        {                                   
            // that uenxpected character is understood as a separator
            // notify latest char received
            quint8 valueFound = asciiToRaw(m_latestAsciiProcessed);

            m_latestAsciiProcessed = LATEST_ASCII_UNSET;

            // notify that value we've just found to the upper layers
            NotifyDelegates(&valueFound, 1);
        }

        break;
    }
    
    } // switch(a_asciiCharacter)
}

quint8 SnifferFileAscii::asciiToRaw(char a_asciiCharacter)
{
    switch(a_asciiCharacter)
    {
    case '0':
        return 0x00;
    case '1':
        return 0x01;
    case '2':
        return 0x02;
    case '3':
        return 0x03;
    case '4':
        return 0x04;
    case '5':
        return 0x05;
    case '6':
        return 0x06;
    case '7':
        return 0x07;
    case '8':
        return 0x08;
    case '9':
        return 0x09;
    case 'A':
    case 'a':
        return 0x0a;
    case 'B':
    case 'b':
        return 0x0b;
    case 'C':
    case 'c':
        return 0x0c;
    case 'D':
    case 'd':
        return 0x0d;
    case 'E':
    case 'e':
        return 0x0e;
    case 'F':
    case 'f':
        return 0x0f;

    default:
        // this is highly unexpected
        Q_ASSERT(0);
        return 0x00;
    } // switch(a_asciiCharacter)
}
