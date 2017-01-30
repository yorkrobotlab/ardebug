#ifndef DATATHREAD_H
#define DATATHREAD_H

#include "QThread"

class DataThread : public QThread
{
    Q_OBJECT

    void run() {
        connectToServer();
    }

signals:
    void dataFromThread(QString data);

private:
    void connectToServer();
};

#endif // DATATHREAD_H
