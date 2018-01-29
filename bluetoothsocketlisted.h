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
public:
    BluetoothSocketListed(QBluetoothAddress addr, int index, QObject *parent = 0);
    ~BluetoothSocketListed();
    QByteArray readLine();
    bool canReadLine();

signals:
    void readyRead_indexed( int index);
private slots:
    void socketReadyRead();


};

#endif // BLUETOOTHSOCKETLISTED_H
