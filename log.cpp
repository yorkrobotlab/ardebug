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

Log::Log()
{
    setLoggingEnabled(false);
    logDirectory = QDir::currentPath();
    std::cout << logDirectory.toStdString() << std::endl;
}

/* setConsoleRef
 * Set up the pointer to the console text widget
 */
void Log::setup(QPlainTextEdit *console, QLabel* dirLabel) {
    this->console = console;
    this->logDirLabel = dirLabel;
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

        if (logDirLabel != NULL) {
            if (logDirectory.length() > 20) {
                logDirLabel->setText(QString("...") + logDirectory.right(20));
            } else {
                logDirLabel->setText(logDirectory);
            }
        }
    }
}

/* logMessage
 * Log a message to the console, if the reference exists,
 * and the text log file, if logging is enabled.
 */
void Log::logMessage(QString message) {
    if (console != NULL) {
        console->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
        console->insertPlainText(message);
    }

    if (isLoggingEnabled()) {
        std::ofstream logFile("log.txt", std::ios_base::app);
        if (logFile.is_open()) {
            logFile << message.toStdString();
            logFile.close();
        }
    }
}
