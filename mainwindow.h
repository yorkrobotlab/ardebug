#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QThread>
#include <QItemSelection>

#include "visualiser.h"
#include "cameracontroller.h"
#include "datamodel.h"

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
    DataModel* dataModel;
    int selectedRobotID;

    int sockfd;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void openUDPSocket(int port);
    void closeUDPSocket(void);

    void startReadingCamera(void);
    void stopReadingCamera(void);

public slots:
    void dataModelUpdate(bool listChanged);

    void robotListSelectionChanged(const QItemSelection &selection);

    void socketOpened(const int &sockfd) { this->sockfd = sockfd; }

private slots:
    void on_testButton_clicked();

    void on_actionExit_triggered();

    void on_actionEnable_Video_changed();

    void on_videoEnChbx_stateChanged();

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;

    void setVideo(bool enabled);
    void updateOverviewTab(void);
};

#endif // MAINWINDOW_H
