#ifndef DATATHREAD_H
#define DATATHREAD_H

#include "QThread"
#include "QUdpSocket"
//#include "QTimer"

class DataThread : public QThread
{
    Q_OBJECT
    QUdpSocket* socket = nullptr;
    volatile bool shouldRun = true;


public:
    DataThread(QObject* parent = nullptr);
    ~DataThread();

    virtual void quit() { this->blockSignals(true); shouldRun = false; }
    virtual void run() override;

public slots:
    void openUDPSocket(int port);

signals:
    void socketOpened(const int &);
    void dataFromThread(const QString &);
};

#endif // DATATHREAD_H
