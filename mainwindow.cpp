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
#include "settings.h"

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

    // Show a message
    ui->statusBar->showMessage("SwarmDebug v0.01", 3000);

    // Show some console text
    ui->consoleText->insertPlainText("Application Started Succesfully.\n");

    // Set up the data model
    dataModel = new DataModel;
    ui->robotList->setModel(dataModel->getRobotList());
    connect(ui->robotList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(robotListSelectionChanged(QItemSelection)));
    ui->robotList->setEditTriggers(QListView::NoEditTriggers);

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
    connect(visualiser, SIGNAL(frameSizeChanged(int, int)), cameraController, SLOT(updateFrameSize(int, int)));

    // Connect tracking position data signal to data model
    connect(cameraController, SIGNAL(posData(QString)), dataModel, SLOT(newData(QString)));
    cameraThread.start();

    // Have the visualiser pass its initial frame size to the camera controller
    visualiser->checkFrameSize();

    ui->imageXDimEdit->setValidator(new QIntValidator(1, 10000, this));
    ui->imageYDimEdit->setValidator(new QIntValidator(1, 10000, this));

    // Start the camera reading immediately
    startReadingCamera();
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
    int idx = selection.indexes().at(0).row();
    if (idx >= 0 || idx < dataModel->getRobotCount()) {
        RobotData* robot = dataModel->getRobotByIndex(idx);

        // Update the selected robot id
        dataModel->selectedRobotID = robot->getID();

        // Show a status bar message
        ui->statusBar->showMessage(robot->getName(), 3000);
    } else {
        dataModel->selectedRobotID = -1;
    }

    // Update the overview tab
    updateOverviewTab();

    // Update the state tab
    updateStateTab();
}

void MainWindow::updateOverviewTab(void) {
    // Get the selected robot
    if (dataModel->selectedRobotID >= 0) {
        RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);

        // Update the overview text
        ui->robotIDLabel->setText(QString::number(robot->getID()));
        ui->robotNameLabel->setText(robot->getName());
        ui->robotStateLabel->setText(robot->getState());
        ui->robotPosLabel->setText("X: " + QString::number(robot->getPos().x) + ", Y: " + QString::number(robot->getPos().y) + ", A: " + QString::number(robot->getAngle()));
    }
}

void MainWindow::updateStateTab(void) {
    // Get the selected robot
    if (dataModel->selectedRobotID >= 0) {
        RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);

        // Update the state lists
        ui->stateList->setModel(robot->getKnownStates());
        ui->stateTransitionList->setModel(robot->getStateTransitionList());
    }
}

void MainWindow::dataModelUpdate(bool listChanged)
{
    // Update the robot list
    if (listChanged) {
        ui->robotList->setModel(dataModel->getRobotList());

        int idx = dataModel->getRobotIndex(dataModel->selectedRobotID, false);

        if (idx != -1) {
            QModelIndex qidx = ui->robotList->model()->index(idx, 0);
            ui->robotList->setCurrentIndex(qidx);
        }
    }

    // Update the overview tab
    updateOverviewTab();
}

/* on_networkListenButton_clicked
 * Slot. Called when the listen for data button is clicked. Toggles between
 * start and stop listening. Opens and closes the UDP socket respectively.
 */
void MainWindow::on_networkListenButton_clicked()
{
    static bool listening = false;
    static int openPort = -1;

    if (!listening) {
        bool ok = false;
        int port = ui->networkPortBox->text().toInt(&ok);

        if (ok) {
            listening = true;
            openUDPSocket(port);
            openPort = port;
            ui->networkListenButton->setText("Stop Listening");
            ui->networkPortBox->setDisabled(true);

            ui->consoleText->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
            ui->consoleText->insertPlainText(QString("Listening for robot data on port ") + QString::number(port) + QString("...\n"));
        }
    } else {
        if (openPort >= 0) {
            listening = false;
            sendClosePacket(openPort);
            openPort = -1;
            ui->networkListenButton->setText("Start Listening");
            ui->networkPortBox->setDisabled(false);

            ui->consoleText->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
            ui->consoleText->insertPlainText("Closing data socket.\n");
        }
    }
}

/* on_networkPortBox_textChanged
 * Called when the user types in the port box on the network tab. Only allow
 * numerical digits, no characters.
 */
void MainWindow::on_networkPortBox_textChanged(const QString &text)
{
    QString newString;

    for (int i = 0; i < text.length(); i++) {
        if (text.at(i).isDigit()) {
            newString.append(text.at(i));
        }
    }

    ui->networkPortBox->setText(newString);
}

void MainWindow::on_imageXDimEdit_textChanged(const QString &arg1)
{
    bool ok = false;
    int w = arg1.toInt(&ok);

    if (ok) {
        Settings::instance()->setCameraImageWidth(w);
    }

    visualiser->checkFrameSize();
}

void MainWindow::on_imageYDimEdit_textChanged(const QString &arg1)
{
    bool ok = false;
    int h = arg1.toInt(&ok);

    if (ok) {
        Settings::instance()->setCameraImageHeight(h);
    }

    visualiser->checkFrameSize();
}
