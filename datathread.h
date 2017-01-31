#ifndef DATATHREAD_H
#define DATATHREAD_H

#include "QThread"
#include "QTimer"

class DataThread : public QObject
{
    Q_OBJECT
    QThread dataThread;
    QTimer * readTimer;
    int sockfd = 0;

public:
    int disconnect = 0;

public slots:
    void connectToServer(const QString param);
    void readDataFromServer(void);
    void disconnectFromServer(void);

signals:
    void dataFromThread(const QString &);
};

#endif // DATATHREAD_H
