/* mainwindow.cpp
 *
 * The top level UI code. Initialises the UI and responds to actions.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datathread.h"
#include "machinevision.h"
#include "datamodel.h"
#include "robotdata.h"
#include "util.h"

#include <sys/socket.h>

#include <QLayout>

/* Constructor.
 * Do UI set up tasks.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Show some default text
    ui->consoleText->appendPlainText("Overview info:\n\nNo robot selected.");

    // Show a message
    ui->statusBar->showMessage("Application Started Successfully.", 3000);

    // Set up the data model
    dataModel = new DataModel;
    ui->robotList->setModel(dataModel->getRobotList());
    connect(ui->robotList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(robotListSelectionChanged(QItemSelection)));

    // Set up the network thread
    DataThread *dataHandler = new DataThread;
    dataHandler->moveToThread(&networkThread);
    connect(&networkThread, SIGNAL(finished()), dataHandler, SLOT(deleteLater()));

    // Connect signals and sockets for starting and stopping the networking
    connect(this, SIGNAL(openUDPSocket(int)), dataHandler, SLOT(openUDPSocket(int)));

    // Connect signals and sockets for transferring the incoming data
    connect(dataHandler, SIGNAL(dataFromThread(QString)), dataModel, SLOT(newData(QString)));
    connect(dataModel, SIGNAL(modelChanged(bool)), this, SLOT(dataModelUpdate(bool)));

    networkThread.start();

    // Intantiate the visualiser
    visualiser = new Visualiser(dataModel);
    visualiser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // Embed the visualiser in the tab
    QHBoxLayout* horizLayout = new QHBoxLayout();
    horizLayout->addWidget(visualiser);
    ui->visualizerTab->setLayout(horizLayout);

    // Instantiate the camera controller and move it to the camera thread
    cameraController = new CameraController();
    cameraController->moveToThread(&cameraThread);
    connect(&cameraThread, SIGNAL(finished()), dataHandler, SLOT(deleteLater()));
    connect(this, SIGNAL(startReadingCamera(void)), cameraController, SLOT(startReadingCamera(void)));
    connect(this, SIGNAL(stopReadingCamera(void)), cameraController, SLOT(stopReadingCamera(void)));

    // Connect image related signals to the visualiser and vice versa
    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(cameraController, SIGNAL(dataFromCamera(cv::Mat)), visualiser, SLOT(showImage(cv::Mat)));
    connect(visualiser, SIGNAL(frameSizeChanged(int)), cameraController, SLOT(updateFrameSize(int)));

    // Connect tracking position data signal to data model
    connect(cameraController, SIGNAL(posData(QString)), dataModel, SLOT(newData(QString)));

    cameraThread.start();

    // Have the visualiser pass its initial frame size to the camera controller
    visualiser->checkFrameSize();

    // Initially no robot selected
    selectedRobotID = -1;
}

/* Destructor.
 * Do UI tear down tasks.
 */
MainWindow::~MainWindow()
{
    sendClosePacket(8888);

    stopReadingCamera();

    delete ui;
    delete dataModel;
    //delete visualiser;
    //delete cameraController;

    networkThread.quit();
    networkThread.wait();

    cameraThread.quit();
    cameraThread.wait();
}

/* on_testButton_clicked
 * Respond to user clicks on 'testButton'.
 */
void MainWindow::on_testButton_clicked()
{
    ui->statusBar->showMessage("Test Button Pressed.", 3000);
    startReadingCamera();
}

/* on_actionExit_triggered
 * The exit menu action was triggered, end the application.
 */
void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::exit();
}

/* on_actionEnableVideo_changed
 * The menu item 'Enable Video' changed state.
 */
void MainWindow::on_actionEnable_Video_changed()
{
    // Update the video enabled state
    setVideo(ui->actionEnable_Video->isChecked());
}

/* on_videoEnChbx_stateChanged
 * The video enabled checkbox on the visualizer settings tab changed state.
 */
void MainWindow::on_videoEnChbx_stateChanged()
{
    // Update the video enabled state.
    setVideo(ui->videoEnChbx->isChecked());
}

/* setVideo
 * Video can be enabled/disabled from multiple places. This function keeps
 * the related controls in sync, and performs the actual enable/disable.
 */
void MainWindow::setVideo(bool enabled) {
    // Update UI controls to new state
    ui->actionEnable_Video->setChecked(enabled);
    ui->videoEnChbx->setChecked(enabled);

    // Display a message
    if(enabled) {
        ui->statusBar->showMessage("Viedo Enabled.", 3000);
    } else {
        ui->statusBar->showMessage("Video Disabled.", 3000);
    }
}

/* on_robotList_selectionChanged
 * A robot was selected from the list
 */
void MainWindow::robotListSelectionChanged(const QItemSelection &selection) {
    // Get the data of the robot selected
    RobotData* robot = dataModel->getRobotByIndex(selection.indexes().at(0).row());

    // Update the selected robot id
    selectedRobotID = robot->getID();

    // Show a status bar message
    ui->statusBar->showMessage(robot->getName(), 3000);

    // Update the overview tab
    updateOverviewTab();
}

void MainWindow::updateOverviewTab(void) {
    // Get the selected robot
    if (selectedRobotID >= 0) {
        RobotData* robot = dataModel->getRobotByID(selectedRobotID);

        // Update the overview text
        ui->robotIDLabel->setText(QString::number(robot->getID()));
        ui->robotNameLabel->setText(robot->getName());
        ui->robotStateLabel->setText(robot->getState());
        ui->robotPosLabel->setText("X: " + QString::number(robot->getPos().x) + ", Y: " + QString::number(robot->getPos().y) + ", A: " + QString::number(robot->getAngle()));
    }
}

/* on_connectButton_clicked()
 * Called when the connect button is clicked. Signals the data thread
 * to open a UDP socket on the port supplied, and listen for data.
 */
void MainWindow::on_connectButton_clicked()
{
    openUDPSocket(8888);
}

/* on_disconnectButton_clicked()
 * Called when the disconnect button is clicked. Signals the data thread
 * to close the UDP socket.
 */
void MainWindow::on_disconnectButton_clicked()
{
    sendClosePacket(8888);
}

void MainWindow::dataModelUpdate(bool listChanged)
{
    // Update the robot list
    if (listChanged) {
        ui->robotList->setModel(dataModel->getRobotList());

        int idx = dataModel->getRobotIndex(selectedRobotID, false);

        if (idx != -1) {
            QModelIndex qidx = ui->robotList->model()->index(idx, 0);
            ui->robotList->setCurrentIndex(qidx);
        }
    }

    // Update the overview tab
    updateOverviewTab();
}
