/* mainwindow.cpp
 *
 * The top level UI code. Initialises the UI and responds to actions.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "settings.h"
#include "log.h"
#include "../Networking/Wifi/datathread.h"
#include "../Networking/Bluetooth/bluetoothdatathread.h"
#include "../Networking/Bluetooth/bluetoothconfig.h"
#include "../DataModel/datamodel.h"
#include "../DataModel/robotdata.h"
#include "../UI/addidmappingdialog.h"
#include "../UI/robotinfodialog.h"

#include <sys/socket.h>

#include <QLayout>
#include <QMediaPlayer>

/* Constructor.
 * Do UI set up tasks.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Show a message
    ui->statusBar->showMessage("Welcome to ARDebug", 3000);

    // Set up the data model
    dataModel = new DataModel;

    // Set up the network thread
    DataThread *dataHandler = new DataThread;
    dataHandler->moveToThread(&networkThread);
    connect(&networkThread, SIGNAL(finished()), dataHandler, SLOT(deleteLater()));

    // Connect signals and sockets for starting and stopping the networking
    connect(this, SIGNAL(openUDPSocket(int)), dataHandler, SLOT(openUDPSocket(int)));    

    // Connect signals and sockets for transferring the incoming data
    connect(dataHandler, SIGNAL(dataFromThread(QString)), dataModel, SLOT(newData(QString)));

    //Set up the bluetoothcommunication
    Bluetoothconfig * btConfig = new Bluetoothconfig();
    BluetoothDataThread *bluetoothHandler = new BluetoothDataThread(btConfig);
    bluetoothHandler->moveToThread(&bluetoothThread);
    connect(&bluetoothThread, SIGNAL(finished()), bluetoothHandler, SLOT(deleteLater()));

    // Connect signals and sockets for starting and stopping bluetooth
    connect(this, SIGNAL(connectBluetooth()), bluetoothHandler, SLOT(connectAllSockets()));
    connect(this, SIGNAL(disconnectBluetooth()), bluetoothHandler, SLOT(disconnectAllSockets()));
    connect(this, SIGNAL(changeStateBluetoothDevice(int)), bluetoothHandler, SLOT(changeSocket(int)));

    // Connect signals and sockets for transferring the incoming data
    connect(bluetoothHandler, SIGNAL(dataFromThread(QString)), dataModel, SLOT(newData(QString)));

    connect(dataModel, SIGNAL(modelChanged(bool)), this, SLOT(dataModelUpdate(bool)));
    networkThread.start();
    bluetoothThread.start();

    // Intantiate the visualiser
    visualiser = new Visualiser(dataModel);
    visualiser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // Embed the visualiser in the tab
    QHBoxLayout* horizLayout = new QHBoxLayout();
    horizLayout->addWidget(visualiser);

    connect(visualiser, SIGNAL(robotSelectedInVisualiser(QString)), this, SLOT(robotSelectedInVisualiser(QString)));

    // Have the visualiser pass its initial frame size to the camera controller
    visualiser->checkFrameSize();

    // Set up the custom data table
    ui->customDataTable->setColumnCount(2);
    ui->customDataTable->setHorizontalHeaderLabels(QStringList("Key") << QString("Value"));
    ui->customDataTable->horizontalHeader()->setStretchLastSection(true);

    // Set up the ID mapping table
    addIDMappingDialog = NULL;
    idMappingTableSetup();
}

/* Destructor.
 * Do UI tear down tasks.
 */
MainWindow::~MainWindow()
{
    // Send a packet to signal the network socket to close
    sendClosePacket(8888);

    // Stop the camera controller
    stopReadingCamera();

    // Release all memory
    delete ui;
    delete dataModel;
    delete visualiser;

    // Delete the id mapping dialog if existing
    if (addIDMappingDialog != NULL) {
        delete addIDMappingDialog;
    }

    // Stop the network thread
    networkThread.quit();
    networkThread.wait();

    // Stop the network thread
    bluetoothThread.quit();
    bluetoothThread.wait();

    // Stop the camera thread
    cameraThread.quit();
    cameraThread.wait();

    // Delete the singletons
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

/* on_robotList_selectionChanged
 * A robot was selected from the list
 */
void MainWindow::robotListSelectionChanged(const QItemSelection &selection) {
    // Get the data of the robot selected
    int idx = selection.indexes().at(0).row();
    if (idx >= 0 || idx < dataModel->getRobotCount()) {
        // Update the selected robot id
        RobotData* robot = dataModel->setSelectedRobot(idx);

        // Show a status bar message
        ui->statusBar->showMessage(robot->getID(), 3000);
    } else {
        dataModel->selectedRobotID = -1;
    }
}

/* on_robotList_doubleClicked
 * Called when an item in the robot list is double clicked, to open the
 * robot info dialog.
 */
void MainWindow::on_robotList_doubleClicked(const QModelIndex &)
{
    // Get the ID
//    int idx = index.row();

//    if (idx >= 0 || idx < dataModel->getRobotCount()) {
//        // Get the robot
//        RobotData* robot = dataModel->getRobotByIndex(idx);

//        // Create and show the robot info dialog
//        RobotInfoDialog* robotInfoDialog = new RobotInfoDialog(robot);
//        QObject::connect(robotInfoDialog, SIGNAL(deleteRobot(int)), dataModel, SLOT(deleteRobot(int)));
//        QObject::connect(robotInfoDialog, SIGNAL(accepted()), this, SLOT(robotDeleted()));
//        robotInfoDialog->show();
//    }
}

/* robotSelectedInVisualiser
 * Slot. Called when a robot is selected by clicking the visualiser. Updates
 * the relevent model data and changes the selected item in the list.
 */
void MainWindow::robotSelectedInVisualiser(QString id) {
    // Update selected ID
    dataModel->selectedRobotID = id;

    // Get the list of strings in the robot list
//    QStringListModel* listModel = (QStringListModel*)ui->robotList->model();
//    QStringList stringList = listModel->stringList();

    // Iterate over the list, checking each string for the selected ID
//    for (int i = 0; i < stringList.size(); i++) {
//        QString str = stringList.at(i);

//        if (str.startsWith(id + ":")) {
            // Update selection
//            ui->robotList->setCurrentIndex(ui->robotList->model()->index(i, 0));
//        }
//    }

    // Update the overview tab
    updateOverviewTab();
}

/* robotDeleted
 * Called when a robot is deleted to update the UI.
 */
void MainWindow::robotDeleted(void) {
    dataModelUpdate(true);
}

/* dataModelUpdate
 * Called when the data model has been updated so that the UI
 * can be updated if necessary.
 *
 * params: listChanged - Indicates whether the contents of the robot list have
 *         potentially changed.
 */
void MainWindow::dataModelUpdate(bool listChanged)
{
    // Update the robot list
    if (listChanged) {
//        ui->robotList->setModel(dataModel->getRobotList());

//        int idx = dataModel->getRobotIndex(dataModel->selectedRobotID, false);

//        if (idx != -1) {
//            QModelIndex qidx = ui->robotList->model()->index(idx, 0);
//            ui->robotList->setCurrentIndex(qidx);
//        }
    }

    // Update the necessary data tabs
    updateOverviewTab();
}

/* updateOverviewTab
 * Updates the contents of the overview tab in response to new data.
 */
void MainWindow::updateOverviewTab(void) {
    // Get the selected robot
    RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);
    if (robot) {
        // Update the overview text
//        ui->robotIDLabel->setText(robot->getID());
//        ui->robotPosLabel->setText("X: " + QString::number(robot->getPos().position.x) + ", Y: " + QString::number(robot->getPos().position.y) + ", A: " + QString::number(robot->getAngle()));
    } else {
//        ui->robotIDLabel->setText("-");
//        ui->robotNameLabel->setText("-");
//        ui->robotStateLabel->setText("-");
//        ui->robotPosLabel->setText("-");
    }
}

/* on_networkListenButton_clicked
 * Slot. Called when the listen for data button is clicked. Toggles between
 * start and stop listening. Opens and closes the UDP socket respectively.
 */
void MainWindow::on_networkListenButton_clicked()
{
    // Statics to monitor listening state and port
    static bool listening = false;
    static int openPort = -1;

    // If not currently listening
    if (!listening) {
        // Parse port number
        bool ok = false;
        int port = 8888;

        if (ok) {
            // Start listening
            listening = true;
            openUDPSocket(port);
            openPort = port;
            //ui->networkListenButton->setText("Stop Listening");
            //ui->networkPortBox->setDisabled(true);

            Log::instance()->logMessage(QString("Listening for robot data on port ") + QString::number(port) + QString("...\n"), true);
        }
    } else {
        if (openPort >= 0) {
            // Stop listening
            listening = false;
            sendClosePacket(openPort);
            openPort = -1;
//            ui->networkListenButton->setText("Start Listening");
//            ui->networkPortBox->setDisabled(false);

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

//    ui->networkPortBox->setText(newString);
}

/* on_imageXDimEdit_textChanged
 * Called when the user types in the image x dimension box on the
 * camera settings tab. Only allow numerical digits, no characters.
 */
void MainWindow::on_imageXDimEdit_textChanged(const QString &arg1)
{
    bool ok = false;
    int w = arg1.toInt(&ok);

    if (ok) {
        Settings::instance()->setCameraImageWidth(w);
    }

    visualiser->checkFrameSize();
}

/* on_imageYDimEdit_textChanged
 * Called when the user types in the image y dimension box on the
 * camera settings tab. Only allow numerical digits, no characters.
 */
void MainWindow::on_imageYDimEdit_textChanged(const QString &arg1)
{
    bool ok = false;
    int h = arg1.toInt(&ok);

    if (ok) {
        Settings::instance()->setCameraImageHeight(h);
    }

    visualiser->checkFrameSize();
}

/* on_visSettingsList_itemClicked
 * Called when an item within the visualiser settings list is clicked.
 * Checks that the visualisation is in the correct enable state.
 */
void MainWindow::on_visSettingsList_itemClicked(QListWidgetItem *item)
{
    QVariant v = item->data(Qt::UserRole);
    VisElement* e = v.value<VisElement *>();

    e->setEnabled(item->checkState() == Qt::Checked);
}

/* on_visSettingsList_itemDoubleClicked
 * Called when an item in the visualiser settings list is double clicked.
 * Displays the detailed visualisation settings if they exist.
 */
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

/* on_logFileButton_clicked
 * Called when the user pressed the log file directory change button
 */
void MainWindow::on_logFileButton_clicked()
{
    Log::instance()->setDirectory(this);
}

/* on_loggingButton_clicked
 * Called when the user clicks the start/stop logging button
 */
void MainWindow::on_loggingButton_clicked()
{
    Log::instance()->setLoggingEnabled(!Log::instance()->isLoggingEnabled());

//    ui->loggingButton->setText(Log::instance()->isLoggingEnabled() ? "Stop Logging" : "Start Logging");
}

/* idMappingTableSetup
 * Initilises the ID mapping table
 */
void MainWindow::idMappingTableSetup(void) {
//    ui->tagMappingTable->setColumnCount(2);

    QStringList headerList;
    headerList.append("ARuCo ID");
    headerList.append("Robot ID");
//    ui->tagMappingTable->setHorizontalHeaderLabels(headerList);
//    ui->tagMappingTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    idMappingUpdate();
}

/* idMappingUpdate
 * Called when the ID mapping has been updated. Udates the table to match.
 */
void MainWindow::idMappingUpdate(void) {
    // Clear the table
//    ui->tagMappingTable->clearContents();

    // Set the new row count
//    ui->tagMappingTable->setRowCount(Settings::instance()->idMapping.size());

    // For each mapping fill a row
    for (size_t i = 0; i < Settings::instance()->idMapping.size(); i++) {
        ArucoIDPair* p = Settings::instance()->idMapping.at(i);

        QTableWidgetItem* arucoID = new QTableWidgetItem(QString::number(p->arucoID));
        QTableWidgetItem* robotID = new QTableWidgetItem(QString::number(p->robotID));

        arucoID->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        robotID->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

//        ui->tagMappingTable->setItem(i, 0, arucoID);
//        ui->tagMappingTable->setItem(i, 1, robotID);
    }
}

/* on_tagMappingDeleteButton_clicked
 * Called when the user presses the button to delete an ID mapping
 */
void MainWindow::on_tagMappingDeleteButton_clicked() {
//    if (ui->tagMappingTable->selectionModel()->hasSelection()) {
//        // Get the selected row
//        QModelIndexList selection = ui->tagMappingTable->selectionModel()->selectedIndexes();

//        for (int i = 0; i < selection.count(); i++) {
//            // Erase the mapping for that row
//            Settings::instance()->idMapping.erase(Settings::instance()->idMapping.begin() + selection.at(i).row());
//        }

//        // Update the table
//        idMappingUpdate();
//    }
}

/* on_tagMappingAddButton_clicked
 * Called when the user presses the button to delete an ID mapping
 */
void MainWindow::on_tagMappingAddButton_clicked()
{
    if (addIDMappingDialog != NULL) {
        delete addIDMappingDialog;
    }

    addIDMappingDialog = new AddIDMAppingDialog();

    if (addIDMappingDialog != NULL) {
        QObject::connect(addIDMappingDialog, SIGNAL(accepted()), this, SLOT(idMappingUpdate(void)));
        addIDMappingDialog->show();
    }
}

/* on_flipImageCheckBox_stateChanged
 * Called when the user changes the image flip setting
 */
void MainWindow::on_flipImageCheckBox_stateChanged(int checked)
{
    Settings::instance()->setImageFlipEnabled(checked == Qt::Checked);
}

/* on_averagePositionCheckBox_stateChanged
 * Called when the user changes the show average position setting
 */
void MainWindow::on_averagePositionCheckBox_stateChanged(int checked)
{
    Settings::instance()->setShowAveragePos(checked == Qt::Checked);
}

/* on_bluetoothListenButton_clicked
 * Called when the user clicks the bluetooth connection button
 */
void MainWindow::on_bluetoothListenButton_clicked()
{

    emit connectBluetooth();
    Log::instance()->logMessage(QString("Connecting to all robots via bluetooth "), true);


}

/* on_bluetoothListenButton_clicked
 * Called when the user clicks the bluetooth disconnection button
 */
void MainWindow::on_bluetoothDisconnectAllButton_clicked()
{
    emit disconnectBluetooth();
    Log::instance()->logMessage(QString("Disconnecting from all robots via bluetooth "), true);
}

void MainWindow::on_bluetoothlist_doubleClicked(const QModelIndex &index)
{
    emit changeStateBluetoothDevice(index.row());
}

void MainWindow::on_angleCorrectionEdit_textChanged(const QString &arg1)
{
    bool ok = false;
    int a = arg1.toInt(&ok);

    if (ok) {
        Settings::instance()->setTrackingAngleCorrection(a);
    }
}
