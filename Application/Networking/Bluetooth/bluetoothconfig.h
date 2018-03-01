#ifndef BLUETOOTHCONFIG_H
#define BLUETOOTHCONFIG_H

#include <QMap>
#include <QString>
#include "bluetoothdevicelistitem.h"
#include <QObject>
#include <QStandardItemModel>

class Bluetoothconfig : public QObject
{
    Q_OBJECT
private:
    std::vector<BluetoothDeviceListItem*> deviceList;
    QStandardItemModel* deviceListModel;
public:
    explicit Bluetoothconfig(QObject *parent = 0);
    ~Bluetoothconfig();
    void getActiveDevices( std::vector<BluetoothDeviceListItem*> * activedeviceList);
    QStandardItemModel* getActiveDeviceList();
    void setItemColour(int index, Qt::GlobalColor colour);
    std::vector<BluetoothDeviceListItem*> * getDeviceList();
    void setDeviceList(std::vector<BluetoothDeviceListItem*> newDeviceList);


private:
    int readFile();
    int writeFile();

public slots:

signals:
    void deviceListChanged(void);
};

#endif //BLUETOOTHCONFIG_H
