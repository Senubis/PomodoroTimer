#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    MainWindow w;
    w.setWindowTitle("番茄钟");
    w.show();

    return a.exec();
}
