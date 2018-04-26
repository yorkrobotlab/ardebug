#ifndef BLUETOOTHSOCKETLISTED_H
#define BLUETOOTHSOCKETLISTED_H

#include <QObject>
#include <QtBluetooth/QBluetoothSocket>
#include <QBluetoothAddress>

class BluetoothSocketListed : public QObject
{
    Q_OBJECT

private:
    int index;
    QBluetoothSocket* socket;
    QBluetoothAddress addr;
public:
    BluetoothSocketListed(QBluetoothAddress addr, int index, QObject *parent = 0);
    ~BluetoothSocketListed();
    QByteArray readLine();
    bool canReadLine();
    void connectSocket();
    void disconnectSocket();
    void changeState();


signals:
    void readyRead_indexed( int index);
    void connectInternalSocket();
    void disconnectInternalSocket();
    void socketDisconnected(const int &);
    void socketConnected(const int &);

private slots:
    void socketReadyRead(void);
    void internalSocketConnector();
    void internalSocketDisconnector();
    void internalSocketDisconnected();
    void internalSocketConnected();





};

#endif // BLUETOOTHSOCKETLISTED_H
