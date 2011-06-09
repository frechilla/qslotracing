#include "snifferfileascii.h"
#include <QFile>
#include <QDebug>

#define LATEST_ASCII_UNSET '\0'

SnifferFileAscii::SnifferFileAscii(QObject *parent):
    QObject(parent),
    m_timer(parent),
    m_filenameList(),
    m_itFilename(m_filenameList.begin()),
    m_currentOpenedFile(0),
    m_openedFileStream(0),
    m_latestAsciiProcessed(LATEST_ASCII_UNSET),
    m_commentedInput(false)
{
    // connect ReadByteFromFile to the timer signals
    // so each time the timer expires that function will
    // get called automatically
    this->connect(&m_timer, SIGNAL(timeout()), SLOT(ReadByteFromFile()));

    ResetParsing();

#if 0
    // add PrintBuffer to the list of slots
    this->connect(this, SIGNAL(bytesRead(QByteArray)), SLOT(PrintBuffer(QByteArray)));
#endif
}

SnifferFileAscii::~SnifferFileAscii()
{
    if (m_openedFileStream != 0)
    {
        delete m_openedFileStream;
    }
    if (m_currentOpenedFile != 0)
    {
        m_currentOpenedFile->close();
        delete m_currentOpenedFile;
    }
}

void SnifferFileAscii::ResetParsing()
{
    if (m_openedFileStream != 0)
    {
        delete m_openedFileStream;
    }
    if (m_currentOpenedFile != 0)
    {
        m_currentOpenedFile->close();
        delete m_currentOpenedFile;
    }

    m_itFilename        = m_filenameList.begin();
    m_currentOpenedFile = 0;
    m_openedFileStream  = 0;
    m_commentedInput    = false;
    m_latestAsciiProcessed = LATEST_ASCII_UNSET;
}

void SnifferFileAscii::AddAsciiFile(const QString& a_filename)
{
    m_filenameList.push_back(a_filename);
    ResetParsing();
}

void SnifferFileAscii::Start(int msec)
{
    if (m_timer.isActive())
    {
        m_timer.stop();

        // restart all pointers  and variables to ensure the parsing
        // process starts off the beginning of the files again
        ResetParsing();
    }

    // start the timer!
    m_timer.start(msec);
}

void SnifferFileAscii::ReadByteFromFile()
{
    // 1. check that the current text stream works
    while ((m_openedFileStream == 0) ||
           (m_openedFileStream->status() != QTextStream::Ok))
    {
        if (m_openedFileStream != 0)
        {
            delete m_openedFileStream;
            m_openedFileStream = 0;
        }

        if (m_itFilename == m_filenameList.end())
        {
            qDebug() << "SnifferFileAscii: There's no more filenames to parse";

            if (m_timer.isActive())
            {
                m_timer.stop();
            }
            return;
        }

        m_itFilename++;
        if (m_itFilename == m_filenameList.end())
        {
            qDebug() << "SnifferFileAscii: Hit the end of the filenames list";

            if (m_timer.isActive())
            {
                m_timer.stop();
            }
            return;
        }

        m_currentOpenedFile = new QFile(*m_itFilename);
        if (!m_currentOpenedFile->open(QIODevice::ReadOnly))
        {
            // could not open this file
            qDebug() << "SnifferFileAscii: Could not open " << *m_itFilename;

            delete m_currentOpenedFile;
            m_currentOpenedFile = 0;
        }

        m_openedFileStream = new QTextStream(m_currentOpenedFile);
    }

    // 2. parse the current textstream to find a byte to be issued
    QChar thisCharacter;
    while(m_openedFileStream->status() == QTextStream::Ok)
    {
        // will exit when the end of the file is reached or there is an
        // error reading from the buffer
        (*m_openedFileStream) >> thisCharacter;

        if (m_commentedInput == false)
        {
            bool rv = ProcessChar(thisCharacter.toUpper().toAscii());

            if (thisCharacter.toAscii() == '#')
            {
                // everything else in this line must be discarded
                m_commentedInput = true;
            }

            if (rv == true)
            {
                // a new signal was emitted. Each call to
                // ReadByteFromFile must return just one byte
                return;
            }
        }
        else if ((thisCharacter.toAscii() == '\r') ||
                 (thisCharacter.toAscii() == '\n') )
        {
            // end of line reached. Resume processing input
            m_commentedInput = false;
        }
    }

    // 3. Hit the end of file. Issue the latest char saved in the buffer
    if ((m_latestAsciiProcessed != LATEST_ASCII_UNSET))
    {
        qDebug() << "SnifferFileAscii: Hit the end of file but a byte hasn't been notified yet!";
        m_latestAsciiProcessed = LATEST_ASCII_UNSET;

        QByteArray ba(reinterpret_cast<const char*>(&m_latestAsciiProcessed), 1);
        emit bytesRead(ba);
    }
}

bool SnifferFileAscii::ProcessChar(char a_asciiCharacter)
{
    quint8 valueFound;

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
            valueFound = asciiToRaw(m_latestAsciiProcessed) << 4;
            valueFound |= asciiToRaw(a_asciiCharacter);
            
            m_latestAsciiProcessed = LATEST_ASCII_UNSET;

            // notify that value we've just found to the upper layers
            QByteArray ba(reinterpret_cast<const char*>(&valueFound), 1);
            emit bytesRead(ba);

            return true;
        }
        break;
    }

    default:
    {
        // unexpected character
        if (m_latestAsciiProcessed != LATEST_ASCII_UNSET) 
        {                                   
            // that unexpected character is understood as a separator
            // notify latest char received
            valueFound = asciiToRaw(m_latestAsciiProcessed);

            m_latestAsciiProcessed = LATEST_ASCII_UNSET;

            // notify that value we've just found to the upper layers
            QByteArray ba(reinterpret_cast<const char*>(&valueFound), 1);
            emit bytesRead(ba);

            return true;
        }
        break;
    }
    
    } // switch(a_asciiCharacter)

    // no signal was issued
    return false;
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

void SnifferFileAscii::PrintBuffer(QByteArray ba)
{
    qDebug() << "Read is : " << ba.size() << " bytes:";
    for (qint32 i = 0; i < ba.size(); i++)
    {
        qDebug() << ba.at(i);
    }
}
