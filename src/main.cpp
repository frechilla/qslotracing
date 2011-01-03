#include <QtGui/QApplication>
#include <iostream>
#include "mainwindow.h"
#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"

#include "delegate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // we need to build the window first
    MainWindow w;

    // SCX protocol analyzer and message factory
    SCXProtoAnalyzer scxAnalyzer;
    SCXMsgFactory msgFactory;

    // conect message factory with proto analyzer
    msgFactory.SetMessageProcessorDelegate(
            MakeDelegate(&SCXProtoAnalyzer::ProcessMsg, &scxAnalyzer));

    // connect output events coming from SCXProtoAnalyzer to the window
    scxAnalyzer.SetEventProcessorDelegate(
            MakeDelegate(&MainWindow::ProcessEvent, &w));

    w.show();

    //////////
    // testing. This should be triggered by the Main Window with some kind of button
#if 0
    // list of files to be read by the ascii sniffer
    QList<QString> fileList;
    fileList.push_back("C:\\file");
    // the message factory must be passed to the ascii
    // sniffer. bytes read by the sniffer will be passed through
    // to the message factory
    SnifferFileAscii asciiSniffer(msgFactory, fileList);

    // start the testing. stuff will be printed on the screen!!
    asciiSniffer.Start();

    std::cout << "Messages discarded: "
              << msgFactory.GetBytesDiscardedCount() << std::endl;
#endif
    // end of testing
    /////////////////

    return a.exec();
}
