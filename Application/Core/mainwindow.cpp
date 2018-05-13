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

//#include "../Networking/Bluetooth/bluetoothconfig.h"

//#include "../Tracking/machinevision.h"

#include "../DataModel/datamodel.h"
#include "../DataModel/robotdata.h"
#include "../UI/addidmappingdialog.h"
#include "../UI/robotinfodialog.h"
#include "../UI/bluetoothconfigdialog.h"

#include <sys/socket.h>

#include <QLayout>
#include <QStandardItemModel>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <sstream>
#include <fstream>

#include "../UI/chartdialog.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


/* Constructor.
 * Do UI set up tasks.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    std::ifstream robotConfigFile{"./RobotConfig.json"};
    if(robotConfigFile)
    {
        std::stringstream configData;
        while(robotConfigFile)
        {
            std::string tmp;
            robotConfigFile >> tmp;
            configData << tmp << " ";
        }
        QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(configData.str()).toUtf8());
        if(!doc.isNull())
        {
            QJsonArray arr = doc.array();
            for(auto item : arr)
            {
                if(!item.isObject())
                    continue;

                auto obj = item.toObject();
                if(!obj.keys().contains("aruco_id") || !obj.keys().contains("robot_id"))
                    continue;

                int arucoId = obj["aruco_id"].toDouble();
                QString robotId = obj["robot_id"].toString();

                arucoNameMapping[arucoId] = robotId;

                std::cout<<"Aruco ID "<<arucoId<<" is now mapped to Robot ID "<<robotId.toStdString()<<std::endl;
            }
        }
    }


#ifdef CVB_CAMERA_PRESENT
    cameraThread = new CVBCameraThread;
#else
    cameraThread = new USBCameraThread;
#endif

    arucoTracker = new ArUco{&arucoNameMapping, cameraThread};


    ui->setupUi(this);

    // Show a message
    ui->statusBar->showMessage("ARDebug v1.1", 3000);

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

    //Set up the bluetoothcommunication
    btConfig = new Bluetoothconfig();
    BluetoothDataThread *bluetoothHandler = new BluetoothDataThread(btConfig);
    bluetoothConfigDialog = NULL;
    bluetoothHandler->moveToThread(&bluetoothThread);
    connect(&bluetoothThread, SIGNAL(finished()), bluetoothHandler, SLOT(deleteLater()));

    // Connect signals and sockets for starting and stopping bluetooth
    connect(this, SIGNAL(connectBluetooth()), bluetoothHandler, SLOT(connectAllSockets()));
    connect(this, SIGNAL(disconnectBluetooth()), bluetoothHandler, SLOT(disconnectAllSockets()));
    connect(this, SIGNAL(changeStateBluetoothDevice(int)), bluetoothHandler, SLOT(changeSocket(int)));


    ui->bluetoothlist->setModel(btConfig->getActiveDeviceList());
    ui->bluetoothlist->setEditTriggers(QListWidget::NoEditTriggers);
    //connect other bluetooth related buttons here

    // Connect signals and sockets for transferring the incoming data
    connect(bluetoothHandler, SIGNAL(dataFromThread(QString)), dataModel, SLOT(newData(QString)));


    connect(dataModel, SIGNAL(modelChanged(bool)), this, SLOT(dataModelUpdate(bool)));
    networkThread.start();
    bluetoothThread.start();



    // Intantiate the visualiser
    visualiser = new Visualiser{dataModel, cameraThread};
    visualiser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    connect(dataModel, SIGNAL(modelChanged(bool)), visualiser, SLOT(refreshVisualisation()));

    // Embed the visualiser in the tab
    QHBoxLayout* horizLayout = new QHBoxLayout();
    horizLayout->addWidget(visualiser);
    ui->visualiserTab->setLayout(horizLayout);

    visualiser->config.populateSettingsList(ui->visSettingsList);

    ui->imageXDimEdit->setValidator(new QIntValidator(1, 10000, this));
    ui->imageYDimEdit->setValidator(new QIntValidator(1, 10000, this));
    ui->angleCorrectionEdit->setValidator(new QIntValidator(-180, 180, this));

    // Set up the custom data table
    ui->customDataTable->setColumnCount(3);
    ui->customDataTable->setHorizontalHeaderLabels(QStringList("Key") << QString("Value") << QString{"Display In Visualiser"});
    ui->customDataTable->setColumnWidth(1, ui->customDataTab->width()*0.8);
    ui->customDataTable->horizontalHeader()->setStretchLastSection(true);

    //set up the chart view
    chart = new QtCharts::QChart();

    //chart->setTitle("robot data");
    //chart->legend()->hide();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    // Arrange the rows vertically
    QVBoxLayout* mainbox = new QVBoxLayout();

    mainbox->addWidget(chartView);

    // Set layout
    ui->chartWidget->setLayout(mainbox);


    // Set up the ID mapping table
    addIDMappingDialog = NULL;
    idMappingTableSetup();

    qRegisterMetaType<cv::Mat>("cv::Mat&");

    connect(arucoTracker, SIGNAL(newRobotPosition(QString, Pose)), dataModel, SLOT(newRobotPosition(QString, Pose)));

    cameraThread->start();

    // Start the camera reading immediately
    startReadingCamera();    
}

/* Destructor.
 * Do UI tear down tasks.
 */
MainWindow::~MainWindow()
{
    // Send a packet to signal the network socket to close
    sendClosePacket(8888);

    std::cout<<"xitinging"<<std::endl;


    // Stop the camera controller
//    emit stopReadingCamera();


    // Stop the network thread
    networkThread.quit();
    networkThread.wait();

    // Stop the network thread
    bluetoothThread.quit();
    bluetoothThread.wait();

    // Stop the camera thread
    cameraThread->quit();
    cameraThread->wait();

    // Release all memory
    delete ui;
    delete dataModel;
    delete visualiser;
    delete chart;

    // Delete the id mapping dialog if existing
    if (addIDMappingDialog != NULL) {
        delete addIDMappingDialog;
    }


    /*if (chartDialog != NULL) {
        delete chartDialog;

    }*/
    if (bluetoothConfigDialog != NULL) {
        delete bluetoothConfigDialog;

    }

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

/* on_actionEnableVideo_changed
 * The menu item 'Enable Video' changed state.
 */
void MainWindow::on_actionEnable_Video_changed()
{
    // Update the video enabled state
    setVideo(ui->actionEnable_Video->isChecked());
}

/* on_videoEnChbx_stateChanged
 * The video enabled checkbox on the visualiser settings tab changed state.
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
        ui->statusBar->showMessage("Video Enabled.", 3000);
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
    if (idx >= 0 || idx < dataModel->getRobotCount())
        dataModel->setSelectedRobot(idx);

    updateCustomData();
}

void MainWindow::updateCustomData()
{
    auto id = dataModel->selectedRobotID;
    if(dataModel->getRobotByID(id))
    {
        RobotData* robot = dataModel->getRobotByID(id);

        std::vector<std::pair<int, int>> selected;
        auto selectedItems = ui->customDataTable->selectedItems();
        for(auto& item : selectedItems)
            selected.push_back({item->row(), item->column()});

        std::stringstream ss;

        const auto& keys = robot->getKeys();

        if(keys.size() != ui->customDataTable->rowCount())
        {
            ui->customDataTable->clear();
            ui->customDataTable->setHorizontalHeaderLabels(QStringList("Key") << QString("Value") << QString{"Display In Visualiser"});
        }

        int i;
        for(i = 0; i < std::min(keys.size(), ui->customDataTable->rowCount()); ++i)
        {
            auto& key = keys[i];
            ui->customDataTable->item(i, 0)->setText(key);

            ss.str("");
            auto type = robot->getValueType(key);

            if(type == String)
                ss<<robot->getStringValue(key).toStdString();

            if(type == Double)
                ss<<robot->getDoubleValue(key);

            if(type == Bool)
                ss<<(robot->getBoolValue(key) ? "True" : "False");

            if(type == Array)
            {
                auto arr = robot->getArrayValue(key);
                ss<<"[ ";
                for(int i = 0; i < arr.size(); ++i)
                {
                    if(i > 0) ss<<"   ";
                    auto item = arr[i];
                    if(item.type == String) ss<<'"'<<item.stringValue.toStdString()<<'"';
                    else if(item.type == Double) ss<<item.doubleValue;
                    else if(item.type == Bool) ss<<(item.boolValue ? "True" : "False");
                    else ss<<"Unsupported";
                }
                ss<<" ]";
            }

            if(type == Object)
            {
                auto obj = robot->getObjectValue(key);
                ss<<"{ ";
                for(auto& key : obj.keys())
                {
                    ss<<key.toStdString()<<": ";
                    auto& item = obj[key];
                    if(item.type == String) ss<<'"'<<item.stringValue.toStdString()<<'"';
                    else if(item.type == Double) ss<<item.doubleValue;
                    else if(item.type == Bool) ss<<(item.boolValue ? "True" : "False");
                    else ss<<"Unsupported";
                    ss<<"   ";
                }
                ss<<" }";
            }
            ui->customDataTable->item(i, 1)->setText(QString::fromStdString(ss.str()));

            QCheckBox* cb = static_cast<QCheckBox*>(ui->customDataTable->cellWidget(i, 2));
            cb->disconnect();
            cb->setChecked(robot->valueShouldBeDisplayed(key));
            connect(cb, &QCheckBox::stateChanged, [=](int sig){ robot->setValueDisplayed(key, sig == 2); });
        }

        for(; i < keys.size(); ++i)
        {
            auto& key = keys[i];
            int newRowIndex = ui->customDataTable->rowCount();
            ui->customDataTable->insertRow(newRowIndex);
            ui->customDataTable->setItem(newRowIndex, 0, new QTableWidgetItem{key});

            ss.str("");
            auto type = robot->getValueType(key);

            if(type == String)
                ss<<robot->getStringValue(key).toStdString();

            if(type == Double)
                ss<<robot->getDoubleValue(key);

            if(type == Bool)
                ss<<(robot->getBoolValue(key) ? "True" : "False");

            if(type == Array)
            {
                auto arr = robot->getArrayValue(key);
                ss<<"[ ";
                for(int i = 0; i < arr.size(); ++i)
                {
                    if(i > 0) ss<<"   ";
                    auto item = arr[i];
                    if(item.type == String) ss<<'"'<<item.stringValue.toStdString()<<'"';
                    else if(item.type == Double) ss<<item.doubleValue;
                    else if(item.type == Bool) ss<<(item.boolValue ? "True" : "False");
                    else ss<<"Unsupported";
                }
                ss<<" ]";
            }

            if(type == Object)
            {
                auto obj = robot->getObjectValue(key);
                ss<<"{ ";
                for(auto& key : obj.keys())
                {
                    ss<<key.toStdString()<<": ";
                    auto& item = obj[key];
                    if(item.type == String) ss<<'"'<<item.stringValue.toStdString()<<'"';
                    else if(item.type == Double) ss<<item.doubleValue;
                    else if(item.type == Bool) ss<<(item.boolValue ? "True" : "False");
                    else ss<<"Unsupported";
                    ss<<"   ";
                }
                ss<<" }";
            }
            ui->customDataTable->setItem(newRowIndex, 1, new QTableWidgetItem{QString::fromStdString(ss.str())});

            QCheckBox* cb = new QCheckBox;
            cb->setChecked(robot->valueShouldBeDisplayed(key));
            connect(cb, &QCheckBox::stateChanged, [=](int sig){ std::cout<<"Checkbox state changed to "<<sig<<std::endl; });
            ui->customDataTable->setCellWidget(newRowIndex, 2, cb);
        }

        for(auto& item : selected)
        {
            ui->customDataTable->item(item.first, item.second)->setSelected(true);
        }
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
    QStringListModel* listModel = (QStringListModel*)ui->robotList->model();
    QStringList stringList = listModel->stringList();

    // Iterate over the list, checking each string for the selected ID
    for (int i = 0; i < stringList.size(); i++) {
        QString str = stringList.at(i);

        if (str == id) {
            // Update selection
            ui->robotList->setCurrentIndex(ui->robotList->model()->index(i, 0));
        }
    }

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
        auto model = dataModel->getRobotList();
        ui->robotList->setModel(model);

        auto stringList = model->stringList();
        auto found = std::find_if(stringList.begin(), stringList.end(), [&](const QString& item){ return item == dataModel->selectedRobotID; });
        if(found != stringList.end())
        {
            QModelIndex qidx = ui->robotList->model()->index(found - stringList.begin(), 0);
            ui->robotList->setCurrentIndex(qidx);
        }
    }

    // Update the necessary data tabs
    updateOverviewTab();
    updateCustomData();
}

/* updateOverviewTab
 * Updates the contents of the overview tab in response to new data.
 */
void MainWindow::updateOverviewTab(void) {
    // Get the selected robot
    RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);
    if (robot) {
        // Update the overview text
        ui->robotIDLabel->setText(robot->getID());
        ui->robotPosLabel->setText("X: " + QString::number(robot->getPos().position.x) + ", Y: " + QString::number(robot->getPos().position.y) + ", A: " + QString::number(robot->getAngle()));
    } else {
        ui->robotIDLabel->setText("-");
        ui->robotNameLabel->setText("-");
        ui->robotStateLabel->setText("-");
        ui->robotPosLabel->setText("-");
    }
}

/* visConfigUpdate
 * Called when the settings of a visualisation have changed, and the
 * visualiser config list should be updated.
 */
void MainWindow::visConfigUpdate(void) {
    visualiser->config.populateSettingsList(ui->visSettingsList);
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
        int port = ui->networkPortBox->text().toInt(&ok);

        if (ok) {
            // Start listening
            listening = true;
            openUDPSocket(port);
            openPort = port;
            ui->networkListenButton->setText("Stop Listening");
            ui->networkPortBox->setDisabled(true);

            Log::instance()->logMessage(QString("Listening for robot data on port ") + QString::number(port) + QString("...\n"), true);
        }
    } else {
        if (openPort >= 0) {
            // Stop listening
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

/* on_robotColoursCheckBox_stateChanged
 * Called when the user changes the robot colour enabled setting.
 */
void MainWindow::on_robotColoursCheckBox_stateChanged()
{
    Settings::instance()->setRobotColourEnabled(ui->robotColoursCheckBox->isChecked());
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

    ui->loggingButton->setText(Log::instance()->isLoggingEnabled() ? "Stop Logging" : "Start Logging");
}

/* idMappingTableSetup
 * Initilises the ID mapping table
 */
void MainWindow::idMappingTableSetup(void) {
    ui->tagMappingTable->setColumnCount(2);

    QStringList headerList;
    headerList.append("ARuCo ID");
    headerList.append("Robot ID");
    ui->tagMappingTable->setHorizontalHeaderLabels(headerList);
    ui->tagMappingTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    idMappingUpdate();
}

/* idMappingUpdate
 * Called when the ID mapping has been updated. Udates the table to match.
 */
void MainWindow::idMappingUpdate(void) {
    // Clear the table
    ui->tagMappingTable->clearContents();

    // Set the new row count
    ui->tagMappingTable->setRowCount(Settings::instance()->idMapping.size());

    // For each mapping fill a row
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

/* on_tagMappingDeleteButton_clicked
 * Called when the user presses the button to delete an ID mapping
 */
void MainWindow::on_tagMappingDeleteButton_clicked() {
    if (ui->tagMappingTable->selectionModel()->hasSelection()) {
        // Get the selected row
        QModelIndexList selection = ui->tagMappingTable->selectionModel()->selectedIndexes();

        for (int i = 0; i < selection.count(); i++) {
            // Erase the mapping for that row
            Settings::instance()->idMapping.erase(Settings::instance()->idMapping.begin() + selection.at(i).row());
        }

        // Update the table
        idMappingUpdate();
    }
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

void MainWindow::on_bluetoothConfigButton_clicked()
{
    if (bluetoothConfigDialog != NULL) {
        delete bluetoothConfigDialog;
        bluetoothConfigDialog = NULL;
        qDebug()<<"deleted Dialog";
    }

    bluetoothConfigDialog = new BluetoothConfigDialog(btConfig);

    if (bluetoothConfigDialog != NULL) {
        //QObject::connect(addIDMappingDialog, SIGNAL(accepted()), this, SLOT(idMappingUpdate(void)));
        bluetoothConfigDialog->show();
    }
}



void MainWindow::on_customDataTable_itemDoubleClicked(QTableWidgetItem *item)
{
     qDebug()<<"doubleClicked";
      QString  dataset =  ui->customDataTable->item(item->row(), 0)->text();


     QMap<QString, int> entryList;

     chart->removeAllSeries();
     QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
     //series->setLabelsVisible(true);




    //get data for chart
     for(int i = 0; i<dataModel->getRobotCount(); i++)
     {
         RobotData* robot = dataModel->getRobotByIndex(i);

         QString value = robot->getStringValue(dataset);

         if (entryList.contains(value))
         {
            entryList[value] = entryList[value] + 1;

         }
         else
         {
            entryList[value] = 1;

         }
         qDebug()<<"robot: " <<i <<  " value: "<< value;


     }
     int counter = 0;
     //create chart from data
     for(const auto& key : entryList.keys())
     {
         series->append(key, entryList[key]);
         //QtCharts::QPieSlice *slice = series->slices().at(counter );

         //slice->setLabelVisible();

         qDebug()<<"data in dialog: " <<key;
         counter ++;

     }


     chart->addSeries(series);


    /* QString  dataset =  ui->customDataTable->item(item->row(), 0)->text();
    qDebug()<<dataset;
    if (chartDialog != NULL) {
        delete chartDialog;
        chartDialog = NULL;
        qDebug()<<"deleted chart Dialog";
    }

    chartDialog = new Chartdialog(dataModel);

    if (chartDialog != NULL) {
        QObject::connect(this, SIGNAL(chartDataSelected(QString )), chartDialog, SLOT(newDataSelected(const QString &)));
        emit chartDataSelected(dataset);

        chartDialog->show();

    }*/
}
