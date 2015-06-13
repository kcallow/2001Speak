#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    /*
//    while(true) {
    consoleApp.waitForReadyRead();
    output
    w.setWindowText(QString::fromAscii(output));
//    }
*/
    return a.exec();
}

