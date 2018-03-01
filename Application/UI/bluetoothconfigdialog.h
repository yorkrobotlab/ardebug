#ifndef BLUETOOTHCONFIGDIALOG_H
#define BLUETOOTHCONFIGDIALOG_H

#include <QObject>
#include <QDialog>
#include <QListView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QMessageBox>
#include "../Networking/Bluetooth/bluetoothconfig.h"


class BluetoothConfigDialog : public QDialog
{
private:
    Q_OBJECT
public:
    explicit BluetoothConfigDialog(Bluetoothconfig* btConfig);
    ~BluetoothConfigDialog();
private:
    QStandardItemModel* deviceListModel ;
    std::vector<BluetoothDeviceListItem*> *deviceList;
    Bluetoothconfig* btConfig;
    QListView* currentList;
    QLineEdit * manualAdditionName;
    QLineEdit * manualAdditionAddress;
    QMessageBox * infoDialog;
    void accept(void);
    void reject(void);


public slots:
    void toggleStatus();
    void deleteEntry();
    void addEntry();

};

#endif // BLUETOOTHCONFIGDIALOG_H
