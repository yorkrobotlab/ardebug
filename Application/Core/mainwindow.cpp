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
#include "../UI/bluetoothconfigdialog.h"

#include <sys/socket.h>

#include <QLayout>
#include <QStandardItemModel>
#include <QCheckBox>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <sstream>
#include <fstream>

#include "../UI/chartdialog.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSlice>
#include <QColor>



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

    // Show some console text
    Log::instance()->logMessage("ARDebug started successfully\n", true);

    // Set up the data model
    dataModel = new DataModel;
    ui->robotList->setModel(dataModel->getRobotList());
    connect(ui->robotList->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(robotListSelectionChanged(QItemSelection)));
    ui->robotList->setEditTriggers(QListView::NoEditTriggers);

    // Set up the network thread
    dataThread = new DataThread{this};
    dataThread->start();

    // Connect signals and sockets for starting and stopping the networking
    connect(this, SIGNAL(openUDPSocket(int)), dataThread, SLOT(openUDPSocket(int)));

    // Connect signals and sockets for transferring the incoming data
    connect(dataThread, SIGNAL(dataFromThread(QString)), dataModel, SLOT(newData(QString)));

    //Set up the bluetoothcommunication
    bluetoothThread.start();
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



    // Intantiate the visualiser
    visualiser = new Visualiser{dataModel, cameraThread};
    visualiser->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    connect(dataModel, SIGNAL(modelChanged(bool)), visualiser, SLOT(refreshVisualisation()));
    connect(dataModel, SIGNAL(modelChanged(bool)), this, SLOT(redrawChart()));
    QObject::connect(this, SIGNAL(updateChart()),this ,SLOT(redrawChart()));

    // Embed the visualiser in the tab
    QHBoxLayout* horizLayout = new QHBoxLayout();
    horizLayout->addWidget(visualiser);
    ui->visualiserTabWidget->setLayout(horizLayout);

    // Set up the custom data table
    ui->customDataTable->setColumnCount(3);
    ui->customDataTable->setHorizontalHeaderLabels(QStringList("Key") << QString("Value") << QString{"Display In Visualiser"});
    ui->customDataTable->setColumnWidth(1, ui->customDataTab->width()*0.8);
    ui->customDataTable->horizontalHeader()->setStretchLastSection(true);
    ui->customDataTable->setEditTriggers(QTableWidget::NoEditTriggers);

    //set up the chart view
    chart = new QtCharts::QChart();

    //chart->setTitle("robot data");
    chart->legend()->hide();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    colourmap[0].setRgb(230,25,75);//red
    colourmap[1].setRgb(60,180,75);//green
    colourmap[2].setRgb(255,225,25);//yellow
    colourmap[3].setRgb(0,130,200);//blue
    colourmap[4].setRgb(245,130,48);//orange
    colourmap[5].setRgb(140,30,180);//purple
    colourmap[6].setRgb(70,240,240);//cyan
    colourmap[7].setRgb(240,30,230);//magenta
    colourmap[8].setRgb(170,110,40);//brown
    colourmap[9].setRgb(0,0,128);//navy



    // Arrange the rows vertically
    QVBoxLayout* mainbox = new QVBoxLayout();

    mainbox->addWidget(chartView);

    // Set layout
    ui->chartWidget->setLayout(mainbox);


    // Set up the ID mapping table
    addIDMappingDialog = NULL;

    qRegisterMetaType<cv::Mat>("cv::Mat&");

    connect(arucoTracker, SIGNAL(newRobotPosition(QString, Pose)), dataModel, SLOT(newRobotPosition(QString, Pose)));
    connect(visualiser, SIGNAL(robotSelectedInVisualiser(QString)), this, SLOT(robotSelectedInVisualiser(QString)));

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
    dataThread->quit();
    dataThread->wait();

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

            if (ui->customDataTable->item(i, 0))
                ui->customDataTable->item(i, 0)->setText(key);
            else
                ui->customDataTable->setItem(i, 0, new QTableWidgetItem{key});


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
            if(ui->customDataTable->item(i, 1))
                 ui->customDataTable->item(i, 1)->setText(QString::fromStdString(ss.str()));
            else
                ui->customDataTable->setItem(i, 1, new QTableWidgetItem{QString::fromStdString(ss.str())});

            QCheckBox* cb = static_cast<QCheckBox*>(ui->customDataTable->cellWidget(i, 2));
            if(cb)
            {
                cb->disconnect();
                cb->setChecked(robot->valueShouldBeDisplayed(key));
                connect(cb, &QCheckBox::stateChanged, [=](int sig){ robot->setValueDisplayed(key, sig == 2); });
            }
            else
            {

                QCheckBox* cb = new QCheckBox;
                cb->setChecked(robot->valueShouldBeDisplayed(key));
                connect(cb, &QCheckBox::stateChanged, [=](int sig){ std::cout<<"Checkbox state changed to "<<sig<<std::endl; });
                ui->customDataTable->setCellWidget(i, 2, cb);

            }
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
            if(ui->customDataTable->item(item.first, item.second))
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
    std::cout<<"Robot selected in visualiser"<<std::endl;
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
    updateCustomData();
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

      chartEntry =  ui->customDataTable->item(item->row(), 0)->text();
      RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);


      chartType =robot->getValueType(chartEntry);

      emit updateChart();





}

 void MainWindow::redrawChart()
 {

     QMap<QString, int> entryList;
     QMap<QString, QColor> colourList;

     int colourCounter = 0;


     chart->removeAllSeries();

     if (chartType==ValueType::String)
     {
        QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
        //get data for chart
         for(int i = 0; i<dataModel->getRobotCount(); i++)
         {
             RobotData* robot = dataModel->getRobotByIndex(i);
              QString value ;

             if (robot->getValueType(chartEntry)==ValueType::String)
                 value = robot->getStringValue(chartEntry);
             else
                 value = "empty";

             if (entryList.contains(value))
             {
                entryList[value] = entryList[value] + 1;
                robot->colour = colourList[value];
             }
             else
             {
                entryList[value] = 1;
                colourList[value]= colourmap[colourCounter%NR_OF_COLOURS];
                robot->colour= colourList[value];
                colourCounter ++;

             }


         }
         int counter = 0;
         //create chart from data
         QFont font("Arial", 8);
         for(const auto& key : entryList.keys())
         {
             QString label = QString("%1 %2").arg(entryList[key]).arg(key);
             series->append(label, entryList[key]);
             QtCharts::QPieSlice *slice = series->slices().at(counter );
             slice->setLabelFont(font);
             slice->setColor(colourList[key]);
             //slice->setLabelVisible();


             counter ++;

         }

        series->setLabelsVisible(true);
         chart->addSeries(series);
    }
    else {
         if (chartType==ValueType::Array)
         {

              for(int i = 0; i<dataModel->getRobotCount(); i++)
              {
                  dataModel->getRobotByIndex(i)->colour= QColor(255,255,255);


              }


            QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
            QtCharts::QBarSet *set = new QtCharts::QBarSet(chartEntry) ;

            series->append(set);

            RobotData* robot = dataModel->getRobotByID(dataModel->selectedRobotID);
            if (robot->getValueType(chartEntry)==ValueType::Array)
            {

                auto arr = robot->getArrayValue(chartEntry);

                for(int i = 0; i < arr.size(); ++i)
                {

                    auto item = arr[i];
                    if(item.type == Double) *set<<item.doubleValue;

                }
                series->setLabelsVisible(true);
                chart->addSeries(series);
                chart->createDefaultAxes();
                chart->axisY()->setMax(200);
            }

         }

         else
            {
             for(int i = 0; i<dataModel->getRobotCount(); i++)
             {
                 dataModel->getRobotByIndex(i)->colour= QColor(255,255,255);


             }
         }



    }




 }

