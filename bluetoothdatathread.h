#ifndef BLUETOOTHDATATHREAD
#define BLUETOOTHDATATHREAD


#include "bluetoothsocketlisted.h"


#define NUMBER_OF_BT_SOCKET 7

class BluetoothDataThread : public QObject
{
    Q_OBJECT
    BluetoothSocketListed *btSocket[NUMBER_OF_BT_SOCKET];
   ~BluetoothDataThread();
    void stop();
public:
    BluetoothDataThread();
    int openSocket(QBluetoothAddress addr);
public slots:    
     void readSocket( int index);
private slots:
    void connected();
    void SocketDisconnected();
    //void SocketError(BluetoothSocket::SocketError);

signals:
    void socketOpened(const int &);
    void dataFromThread(const QString &);
};

#endif // BLUETOOTHDATATHREAD

