/* main.cpp
 *
 * Entry point of application.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "mainwindow.h"
#include <QApplication>

// Required for passing cv image type between threads in signals.
Q_DECLARE_METATYPE(cv::Mat)

int main(int argc, char *argv[])
{
    // Show QT Application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
