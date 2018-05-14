#ifndef DATATHREAD_H
#define DATATHREAD_H

#include "QThread"
//#include "QTimer"

class DataThread : public QThread
{
    Q_OBJECT
    int sockfd = 0;
    volatile bool shouldRun = true;

    ~DataThread();

public:
    virtual void quit() { this->blockSignals(true); shouldRun = false; }
    virtual void run() override;

public slots:
    void openUDPSocket(int port);

signals:
    void socketOpened(const int &);
    void dataFromThread(const QString &);
};

#endif // DATATHREAD_H
