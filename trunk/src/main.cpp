#include <QtGui/QApplication>
#include <iostream>
#include "mainwindow.h"
#include "snifferfileascii.h"
#include "scxmsgfactory.h"
#include "scxproto.h"


int main(int argc, char *argv[])
{
    //////////
    // testing
    SCXMsgFactory msgFactory;

    // set the function who will be processing the factory's msgs
    msgFactory.SetMessageProcessorFunction(&scxproto::ProcessMsg);

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

    // end of testing
    /////////////////

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
