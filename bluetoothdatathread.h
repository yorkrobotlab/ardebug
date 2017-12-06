#ifndef BLUETOOTHDATATHREAD
#define BLUETOOTHDATATHREAD



//#include "QTimer"
#include <QtBluetooth/QBluetoothSocket>

class BluetoothDataThread : public QObject
{
    Q_OBJECT
    //QTimer* readTimer;
    QBluetoothSocket *btSocket[6];
    QString dest = "00:06:66:84:93:1E";
    QString dest2 = "00:06:66:60:29:1D";
   ~BluetoothDataThread();
    void stop();
public:
    BluetoothDataThread();
public slots:
    void openSocket();
     void readSocket();
private slots:
    void connected();


signals:
    void socketOpened(const int &);
    void dataFromThread(const QString &);
};

#endif // BLUETOOTHDATATHREAD

