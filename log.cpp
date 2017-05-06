/* log.cpp
 *
 * This class encapsulates logging functionality, including printing to the
 * console as well as the text log file.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "log.h"
#include <iostream>
#include <fstream>

#include <QDir>
#include <QFileDialog>
#include <QTime>

Log::Log()
{
    setLoggingEnabled(false);
    logDirectory = QDir::currentPath();
}

/* setConsoleRef
 * Set up the pointer to the console text widget
 */
void Log::setup(QPlainTextEdit *console, QLabel* dirLabel) {
    this->console = console;
    this->logDirLabel = dirLabel;
    updateDirectoryLabel();
}

bool Log::isLoggingEnabled(void) {
    return loggingEnabled;
}

void Log::setLoggingEnabled(bool enabled) {
    this->loggingEnabled = enabled;
}

QString Log::getDirectory(void) {
    return logDirectory;
}

void Log::setDirectory(QMainWindow* mainWindow) {
    QFileDialog fileDialog(mainWindow);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setDirectory(logDirectory);

    if (fileDialog.exec()) {
        QStringList result = fileDialog.selectedFiles();
        logDirectory = result.at(0);

        updateDirectoryLabel();
    }
}

void Log::updateDirectoryLabel(void) {
    if (logDirLabel != NULL) {
        if (logDirectory.length() > 20) {
            logDirLabel->setText(QString("...") + logDirectory.right(20));
        } else {
            logDirLabel->setText(logDirectory);
        }
    }
}

/* logMessage
 * Log a message to the console, if the reference exists,
 * and the text log file, if logging is enabled.
 */
void Log::logMessage(QString message, bool toConsole) {
    if (message.endsWith("\n")) {
        message.remove(message.length() - 1, 1);
    }

    if (console != NULL && toConsole) {
        console->appendPlainText(message);
    }

    if (isLoggingEnabled()) {
        std::ofstream logFile(logDirectory.toStdString() + "/log.txt", std::ios_base::app);
        if (logFile.is_open()) {
            logFile << QTime::currentTime().toString("HH:mm:ss:zzz").toStdString() << " - " << message.toStdString() << std::endl;
            logFile.close();
        }
    }
}
