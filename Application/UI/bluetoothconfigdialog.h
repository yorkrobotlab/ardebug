#ifndef BLUETOOTHCONFIGDIALOG_H
#define BLUETOOTHCONFIGDIALOG_H

#include <QObject>
#include <QDialog>
#include <QListView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QBluetoothDeviceDiscoveryAgent>
#include "../Networking/Bluetooth/bluetoothconfig.h"


class BluetoothConfigDialog : public QDialog
{
private:
    Q_OBJECT
public:
    explicit BluetoothConfigDialog(Bluetoothconfig* btConfig);
    ~BluetoothConfigDialog();
private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QStandardItemModel* deviceListModel ;
    QStandardItemModel* scanListModel ;
    std::vector<BluetoothDeviceListItem*> *deviceList;
    Bluetoothconfig* btConfig;
    QListView* currentList;
    QListView* scanList;
    QLineEdit * manualAdditionName;
    QLineEdit * manualAdditionAddress;
    QMessageBox * infoDialog;
    QPushButton* scanButton;
    void accept(void);
    void reject(void);


private slots:
    void startScan();
    void scanFinished();
    void addScannedEntry();


public slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void toggleStatus();
    void deleteEntry();
    void addEntry();

};

#endif // BLUETOOTHCONFIGDIALOG_H
