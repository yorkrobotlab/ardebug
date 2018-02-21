#ifndef BLUETOOTHDATATHREAD
#define BLUETOOTHDATATHREAD


#include "bluetoothsocketlisted.h"
#include "bluetoothconfig.h"


#define NUMBER_OF_BT_SOCKET 7

class BluetoothDataThread : public QObject
{
    Q_OBJECT
    BluetoothSocketListed *btSocket[NUMBER_OF_BT_SOCKET];
    Bluetoothconfig * btConfig;
   ~BluetoothDataThread();
    void stop();
     int openSocket(QBluetoothAddress addr);
public:
    BluetoothDataThread(Bluetoothconfig * btConfig);

public slots:
     void readSocket( int index);
     void updateSocketList(void);
     void disconnectAllSockets();
     void connectAllSockets();
     void changeSocket(int index);
private slots:
    void SocketDisconnected(const int &);
    void SocketConnected(const int &);
    //void SocketError(BluetoothSocket::SocketError);

signals:
    void socketOpened(const int &);
    void dataFromThread(const QString &);
    //signals for socket open and close
};

#endif // BLUETOOTHDATATHREAD










