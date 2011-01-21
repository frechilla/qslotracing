#include <QtGui/QApplication>
#include <iostream>
#include "mainwindow.h"
#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"

#include "delegate.h"
#include "recorderbinary.h"

int main(int argc, char *argv[])
{

    //////////
    // testing the binary recorder
#if 0
    QList<QString> fileList;
    fileList.push_back("C:\\file");
    SnifferFileAscii asciiSniffer(fileList);

    // binary recorder will be processing messages sniffed
    // by the asciiSniffer
    RecorderBinary recorder("record", 1024); // rotate every kilobyte
    asciiSniffer.SetProcessorDelegate(
        MakeDelegate(&RecorderBinary::Dump, &recorder));

    // start the testing. stuff will be printed on the screen!!
    asciiSniffer.Start();
#endif
    // end of testing
    /////////////////


    QApplication a(argc, argv);
    // we need to build the window first
    MainWindow w;

    w.show();

    //////////
    // testing. This should be triggered by the Main Window with some kind of button
#if 0

    // list of files to be read by the ascii sniffer
    QList<QString> fileList;
    fileList.push_back("C:\\file");
    // building up the ascii sniffer
    SnifferFileAscii asciiSniffer(fileList);

    // SCX protocol analyzer and message factory
    SCXProtoAnalyzer scxAnalyzer;
    SCXMsgFactory msgFactory;

    // SCXMsgFactory will be processing all bytes sniffed
    // by the asciiSniffer
    asciiSniffer.SetProcessorDelegate(
        MakeDelegate(&SCXMsgFactory::Parse, &msgFactory));
    
    // connect message factory with proto analyzer
    msgFactory.SetMessageProcessorDelegate(
            MakeDelegate(&SCXProtoAnalyzer::ProcessMsg, &scxAnalyzer));

    // connect output events coming from SCXProtoAnalyzer to the window
    scxAnalyzer.SetEventProcessorDelegate(
            MakeDelegate(&MainWindow::ProcessEvent, &w));


    // start the testing. stuff will be printed on the screen!!
    asciiSniffer.Start();

    std::cout << "Messages discarded: "
              << msgFactory.GetBytesDiscardedCount() << std::endl;
#endif
    // end of testing
    /////////////////

    return a.exec();
}
