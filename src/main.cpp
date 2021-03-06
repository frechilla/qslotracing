#include <QtGui/QApplication>
#include <iostream>
#include "mainwindow.h"
#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"

#include "recorderbinary.h"
#include "snifferserial.h"

int main(int argc, char *argv[])
{

    //////////
    // the following code stores all data sniffed off a serial interface into
    // a set of files which maximum size is set to 1mb and which name starts
    // by the prefix "serial"
#if 0
    SnifferSerial serialSniffer;
    RecorderBinary recorder("record", 1048576); // rotate every megabyte
    
    // conect sniffer output to recorder input
    serialSniffer.AddProcessorDelegate(
        MakeDelegate(&RecorderBinary::Dump, &recorder));
        
    // set up and open serial interface
    serialSniffer.SetDeviceName("COM6");
    serialSniffer.SetBaudRate(AbstractSerial::BaudRate115200);
    serialSniffer.SetDataBits(AbstractSerial::DataBits8);
    serialSniffer.SetParity(AbstractSerial::ParityNone);
    serialSniffer.SetStopBits(AbstractSerial::StopBits1);
    serialSniffer.SetFlowControl(AbstractSerial::FlowControlOff);
    serialSniffer.OpenSerial();
    
    // bytes sniffed off the serial interface will be dumped into file using 
    // the signnal&slot mechanism while the application is opened
    // keep looping while stuff is being dumped into file
    while(true)
    {
        ;
    }
    
    // close the serial interface
    serialSniffer.Close();
#endif
    // end of serial sniffing code
    //////////
    
    //////////
    // testing binary recorder
#if 0
    QList<QString> fileList;
    fileList.push_back("C:\\file");
    SnifferFileAscii asciiSniffer(fileList);

    // binary recorder will be processing messages sniffed
    // by the asciiSniffer
    RecorderBinary recorder("record", 1024); // rotate every kilobyte
    asciiSniffer.AddProcessorDelegate(
        MakeDelegate(&RecorderBinary::Dump, &recorder));

    // start the testing. stuff will be printed on the screen!!
    asciiSniffer.Start();
#endif
    // end of testing binary recorder
    /////////////////


    QApplication a(argc, argv);
    // we need to build the window first
    MainWindow w;

    w.move(0,0);
    w.show();

    return a.exec();
}
