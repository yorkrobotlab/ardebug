#ifndef DATATHREAD_H
#define DATATHREAD_H

#include "QThread"
#include "QTimer"

class DataThread : public QObject
{
    Q_OBJECT
    QTimer* readTimer;
    int sockfd = 0;

public slots:
    void openUDPSocket(int port);
    void closeUDPSocket(void);

private slots:
    void listenForPacket(void);

signals:
    void dataFromThread(const QString &);
};

#endif // DATATHREAD_H
