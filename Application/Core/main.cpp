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

#include "../Visualiser/viselement.h"

// Allocate and initialise the singleton pointers
Settings *Settings::s_instance = 0;

/* main
 * Entry point for the whole application.
 */
int main(int argc, char *argv[])
{
    // Show QT Application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
