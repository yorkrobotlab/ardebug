#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QThread>

#include "visualiser.h"
#include "cameracontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread networkThread;
    QThread cameraThread;
    Visualiser* visualiser;
    CameraController* cameraController;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void openUDPSocket(int port);
    void closeUDPSocket(void);

    void startReadingCamera(void);
    void stopReadingCamera(void);

public slots:
    void on_dataFromThread(const QString data);

private slots:
    void on_testButton_clicked();

    void on_actionExit_triggered();

    void on_actionEnable_Video_changed();

    void on_videoEnChbx_stateChanged();

    void on_robotList_clicked(const QModelIndex &index);

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;

    void setVideo(bool enabled);
};

#endif // MAINWINDOW_H
