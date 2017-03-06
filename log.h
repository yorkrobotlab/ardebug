#ifndef LOG_H
#define LOG_H

#include "stddef.h"
#include <QString>
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QLabel>

class Log
{
    static Log* s_instance;

    QPlainTextEdit* console;

    bool loggingEnabled;
    QString logDirectory;
    QLabel* logDirLabel;

    Log();

public:
    static Log* instance() {
        if (s_instance == NULL) {
            s_instance = new Log;
        }

        return s_instance;
    }

    static void deleteInstance() {
        if (s_instance != NULL) {
            delete s_instance;
            s_instance = 0;
        }
    }

    void setup(QPlainTextEdit* console, QLabel* dirLabel);
    void logMessage(QString message, bool toConsole);

    bool isLoggingEnabled(void);
    void setLoggingEnabled(bool enabled);

    QString getDirectory(void);
    void setDirectory(QMainWindow* mainWindow);
};

#endif // LOG_H
