#ifndef LOG_H
#define LOG_H

#include "stddef.h"
#include <QString>
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QLabel>

class Log
{
    QPlainTextEdit* console;

    bool loggingEnabled;
    QString logDirectory;
    QLabel* logDirLabel;

    Log();

public:
    static Log* instance() {
        static Log instance;
        return &instance;
    }

    void setup(QPlainTextEdit* console, QLabel* dirLabel);
    void logMessage(QString message, bool toConsole);

    bool isLoggingEnabled(void);
    void setLoggingEnabled(bool enabled);

    QString getDirectory(void);
    void setDirectory(QMainWindow* mainWindow);
    void updateDirectoryLabel(void);
};

#endif // LOG_H
