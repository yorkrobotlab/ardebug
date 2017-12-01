#ifndef BLUETOOTHDATATHREAD
#define BLUETOOTHDATATHREAD


//#include "QThread"
#include "QTimer"
#include <QtBluetooth/QBluetoothSocket>
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/rfcomm.h>
class BluetoothDataThread : public QObject
{
    Q_OBJECT
    QTimer* readTimer;
    QBluetoothSocket *btSocket;
    QString dest = "00:06:66:84:93:1E";

   ~BluetoothDataThread();
    void stop();
public:
    BluetoothDataThread();
public slots:
    void openSocket();
     void readSocket();
private slots:
    void connected();

private slots:
    void listenForPacket(void);


signals:
    void socketOpened(const int &);
    void dataFromThread(const QString &);
};

#endif // BLUETOOTHDATATHREAD

