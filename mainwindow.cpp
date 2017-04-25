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
#include "log.h"
#include "addidmappingdialog.h"

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
    Log::instance()->setup(ui->consoleText, ui->loggingFileLabel);
    Log::instance()->logMessage("Application Started Succesfully.\n", true);

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
    connect(&cameraThread, SIGNAL(finished()), cameraController, SLOT(deleteLater()));
    connect(this, SIGNAL(startReadingCamera(void)), cameraController, SLOT(startReadingCamera(void)));
    connect(this, SIGNAL(stopReadingCamera(void)), cameraController, SLOT(stopReadingCamera(void)));

    // Connect image related signals to the visualiser and vice versa
    qRegisterMetaType< cv::Mat >("cv::Mat");
    connect(cameraController, SIGNAL(dataFromCamera(cv::Mat)), visualiser, SLOT(showImage(cv::Mat)));
    connect(visualiser, SIGNAL(frameSizeChanged(int, int)), cameraController, SLOT(updateFrameSize(int, int)));
    connect(visualiser, SIGNAL(robotSelectedInVisualiser(int)), this, SLOT(robotSelectedInVisualiser(int)));

    // Connect tracking position data signal to data model
    connect(cameraController, SIGNAL(posData(QString)), dataModel, SLOT(newData(QString)));
    cameraThread.start();

    // Have the visualiser pass its initial frame size to the camera controller
    visualiser->checkFrameSize();

    visualiser->config.populateSettingsList(ui->visSettingsList);

    ui->imageXDimEdit->setValidator(new QIntValidator(1, 10000, this));
    ui->imageYDimEdit->setValidator(new QIntValidator(1, 10000, this));

    // Initalise the IR data view
    irDataView = new IRDataView(dataModel);

    QVBoxLayout* vertLayout = new QVBoxLayout();
    vertLayout->addWidget(irDataView);
    ui->proximityTab->setLayout(vertLayout);

    // Set up the custom data table
    ui->customDataTable->setColumnCount(2);
    ui->customDataTable->setHorizontalHeaderLabels(QStringList("Key") << QString("Value"));
    ui->customDataTable->horizontalHeader()->setStretchLastSection(true);

    // Set up the ID mapping table
    idMappingTableSetup();

    // Start the camera reading immediately
    startReadingCamera();    

    // Initialise the testing window to null
    testingWindow = NULL;
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
    delete visualiser;

    if (testingWindow != NULL) {
        delete testingWindow;
    }

    networkThread.quit();
    networkThread.wait();

    cameraThread.quit();
    cameraThread.wait();

    Settings::deleteInstance();
    Log::deleteInstance();
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
    // Update setting
    Settings::instance()->setVideoEnabled(enabled);

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

    // Update the proximiy sensor tab
    updateProximityTab();
}

/* robotSelectedInVisualiser
 * Slot. Called when a robot is selected by clicking the visualiser. Updates
 * the relevent model data and changes the selected item in the list.
 */
void MainWindow::robotSelectedInVisualiser(int id) {
    // Update selected ID
    dataModel->selectedRobotID = id;

    // Get the list of strings in the robot list
    QStringListModel* listModel = (QStringListModel*)ui->robotList->model();
    QStringList stringList = listModel->stringList();

    // Iterate over the list, checking each string for the selected ID
    for (int i = 0; i < stringList.size(); i++) {
        QString str = stringList.at(i);

        if (str.startsWith(QString::number(id))) {
            // Update selection
            ui->robotList->setCurrentIndex(ui->robotList->model()->index(i, 0));
        }
    }

    // Update the overview tab
    updateOverviewTab();

    // Update the state tab
    updateStateTab();

    // Update the proximiy sensor tab
    updateProximityTab();

    updateCustomDataTab();
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

    // Update the proximity sensor tab
    updateProximityTab();

    updateCustomDataTab();
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

void MainWindow::updateProximityTab(void) {
    irDataView->repaint();
}

void MainWindow::updateCustomDataTab(void) {
    // Get the selected robot
    if (dataModel->selectedRobotID >= 0) {
        RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);

        robot->populateCustomDataTable(ui->customDataTable);
    }
}

void MainWindow::visConfigUpdate(void) {
    visualiser->config.populateSettingsList(ui->visSettingsList);
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

            Log::instance()->logMessage(QString("Listening for robot data on port ") + QString::number(port) + QString("...\n"), true);
        }
    } else {
        if (openPort >= 0) {
            listening = false;
            sendClosePacket(openPort);
            openPort = -1;
            ui->networkListenButton->setText("Start Listening");
            ui->networkPortBox->setDisabled(false);

            Log::instance()->logMessage("Closing data socket.\n", true);
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

void MainWindow::on_visSettingsList_itemClicked(QListWidgetItem *item)
{
    QVariant v = item->data(Qt::UserRole);
    VisElement* e = v.value<VisElement *>();

    e->setEnabled(item->checkState() == Qt::Checked);
}

void MainWindow::on_visSettingsList_itemDoubleClicked(QListWidgetItem *item)
{
    QVariant v = item->data(Qt::UserRole);
    VisElement* e = v.value<VisElement *>();

    QDialog* settingsDialog = e->getSettingsDialog();

    if (settingsDialog != NULL) {
        QObject::connect(settingsDialog, SIGNAL(accepted()), this, SLOT(visConfigUpdate(void)));
        settingsDialog->show();
    }
}

void MainWindow::on_robotColoursCheckBox_stateChanged()
{
    Settings::instance()->setRobotColourEnabled(ui->robotColoursCheckBox->isChecked());
    dataModel->resetRobotColours();
}

void MainWindow::on_logFileButton_clicked()
{
    Log::instance()->setDirectory(this);
}

void MainWindow::on_loggingButton_clicked()
{
    Log::instance()->setLoggingEnabled(!Log::instance()->isLoggingEnabled());

    ui->loggingButton->setText(Log::instance()->isLoggingEnabled() ? "Stop Logging" : "Start Logging");
}

void MainWindow::on_actionTesting_Window_triggered()
{
    // If no testing window exists yet, create one
    if (testingWindow == NULL) {
        testingWindow = new TestingWindow();
    }

    // If the testing window exists show it
    if (testingWindow != NULL) {
        testingWindow->show();
    }
}

void MainWindow::idMappingTableSetup(void) {
    ui->tagMappingTable->setColumnCount(2);

    QStringList headerList;
    headerList.append("ARuCo ID");
    headerList.append("Robot ID");
    ui->tagMappingTable->setHorizontalHeaderLabels(headerList);
    ui->tagMappingTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    idMappingUpdate();
}

void MainWindow::idMappingUpdate(void) {
    ui->tagMappingTable->clearContents();

    ui->tagMappingTable->setRowCount(Settings::instance()->idMapping.size());

    for (size_t i = 0; i < Settings::instance()->idMapping.size(); i++) {
        ArucoIDPair* p = Settings::instance()->idMapping.at(i);

        QTableWidgetItem* arucoID = new QTableWidgetItem(QString::number(p->arucoID));
        QTableWidgetItem* robotID = new QTableWidgetItem(QString::number(p->robotID));

        arucoID->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        robotID->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        ui->tagMappingTable->setItem(i, 0, arucoID);
        ui->tagMappingTable->setItem(i, 1, robotID);
    }
}

void MainWindow::on_tagMappingDeleteButton_clicked() {
    if (ui->tagMappingTable->selectionModel()->hasSelection()) {
        QModelIndexList selection = ui->tagMappingTable->selectionModel()->selectedIndexes();

        for (int i = 0; i < selection.count(); i++) {
            Settings::instance()->idMapping.erase(Settings::instance()->idMapping.begin() + selection.at(i).row());
        }

        idMappingUpdate();
    }
}

void MainWindow::on_tagMappingAddButton_clicked()
{
    QDialog* addIDMappingDialog = new AddIDMAppingDialog();

    if (addIDMappingDialog != NULL) {
        QObject::connect(addIDMappingDialog, SIGNAL(accepted()), this, SLOT(idMappingUpdate(void)));
        addIDMappingDialog->show();
    }
}
