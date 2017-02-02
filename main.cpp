/* main.cpp
 *
 * Entry point of application.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Show QT Application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
