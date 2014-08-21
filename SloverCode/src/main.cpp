#include <QApplication>
#include "MainWindow.h"
int main(int argc, char **argv)
{
    //make an instance of the QApplication
    QApplication a(argc,argv);

    //Create a new Main Window
    MainWindow w;

    //Show it
    w.show();

    //Hand control over to QT Framework
    return a.exec();
}
