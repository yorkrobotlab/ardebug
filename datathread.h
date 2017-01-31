#ifndef DATATHREAD_H
#define DATATHREAD_H

#include "QThread"

class DataThread : public QObject
{
    Q_OBJECT
    QThread dataThread;

public slots:
    void connectToServer(const QString &params);

signals:
    void dataFromThread(QString data);
};

#endif // DATATHREAD_H
