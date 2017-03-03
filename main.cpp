/* main.cpp
 *
 * Entry point of application.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "mainwindow.h"
#include <QApplication>

#include "settings.h"
#include "log.h"

#include "viselement.h"

// Required for passing cv image type between threads in signals.
Q_DECLARE_METATYPE(cv::Mat)

// Allocate and initialise the singleton pointers
Settings *Settings::s_instance = 0;
Log *Log::s_instance = 0;

int main(int argc, char *argv[])
{
    // Show QT Application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
