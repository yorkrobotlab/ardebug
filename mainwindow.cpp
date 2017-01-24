/* mainwindow.cpp
 *
 * The top level UI code. Initialises the UI and responds to actions.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Constructor.
 * Do UI set up tasks.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Show some default text
    ui->overviewText->appendPlainText("Overview info:\n\nNo robot selected.");

    // Show a message
    ui->statusBar->showMessage("Application Started Successfully.", 3000);

    // Create a data model for the robot list
    QStringListModel *robotListModel = new QStringListModel();

    // Create a string list of robots
    QStringList robotList;
    robotList.append("Robot 1");
    robotList.append("Robot 2");
    robotList.append("Robot 3");

    // Apply the list to the model and the model to the view
    robotListModel->setStringList(robotList);
    ui->robotList->setModel(robotListModel);
}

/* Destructor.
 * Do UI tear down tasks.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/* on_testButton_clicked
 * Respond to user clicks on 'testButton'. Displays a test message.
 */
void MainWindow::on_testButton_clicked()
{
    ui->statusBar->showMessage("Test Button Pressed.", 3000);
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

/* on_robotList_clicked
 * A robot was selected from the list
 */
void MainWindow::on_robotList_clicked(const QModelIndex &index)
{
    // Get the name of the robot selected
    QStringListModel *model = (QStringListModel *)ui->robotList->model();
    QString robotName = model->stringList().at(index.row());

    // Update the overview text
    ui->overviewText->clear();
    ui->overviewText->appendPlainText("Overview info:\n");
    ui->overviewText->appendPlainText(robotName);

    // Show a status bar message
    ui->statusBar->showMessage(robotName, 3000);
}
