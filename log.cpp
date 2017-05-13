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

/* Constructor
 * Initialise log settings
 */
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

/* isLoggingEnabled
 * Return status of logging enabled setting
 */
bool Log::isLoggingEnabled(void) {
    return loggingEnabled;
}

/* setLoggingEnabled
 * Change status of logging enabled setting
 */
void Log::setLoggingEnabled(bool enabled) {
    this->loggingEnabled = enabled;
}

/* getDirectory
 * Return the current logging directory
 */
QString Log::getDirectory(void) {
    return logDirectory;
}

/* setDirectory
 * Show a dialog for choosing a new logging directory
 */
void Log::setDirectory(QMainWindow* mainWindow) {
    // Create the dialog. Initialise with current directory.
    QFileDialog fileDialog(mainWindow);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setDirectory(logDirectory);

    // Show dialog
    if (fileDialog.exec()) {
        // If a results is returned update the directory
        QStringList result = fileDialog.selectedFiles();
        logDirectory = result.at(0);

        // Update the label
        updateDirectoryLabel();
    }
}

/* updateDirectoryLabel
 * Update the UI label stating the log directory
 */
void Log::updateDirectoryLabel(void) {
    if (logDirLabel != NULL) {
        // Limit length to 20 characters
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
    // Remove newline character from string end
    if (message.endsWith("\n")) {
        message.remove(message.length() - 1, 1);
    }

    // If necessary append to the console
    if (console != NULL && toConsole) {
        console->appendPlainText(message);
    }

    // If logging is enabled output the log message to the text file
    if (isLoggingEnabled()) {
        std::ofstream logFile(logDirectory.toStdString() + "/log.txt", std::ios_base::app);
        if (logFile.is_open()) {
            logFile << QTime::currentTime().toString("HH:mm:ss:zzz").toStdString() << " - " << message.toStdString() << std::endl;
            logFile.close();
        }
    }
}
