#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->overviewText->appendPlainText("Overview info");

    ui->statusBar->showMessage("Application Started Successfully.", 3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_testButton_clicked()
{
    ui->statusBar->showMessage("Test Button Pressed.", 3000);
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::exit();
}

void MainWindow::on_actionEnable_Video_changed()
{
    setVideo(ui->actionEnable_Video->isChecked());
}

void MainWindow::on_videoEnChbx_stateChanged(int arg1)
{
    setVideo(ui->videoEnChbx->isChecked());
}

void MainWindow::setVideo(bool enabled) {
    ui->actionEnable_Video->setChecked(enabled);
    ui->videoEnChbx->setChecked(enabled);

    if(enabled) {
        ui->statusBar->showMessage("Viedo Enabled.", 3000);
    } else {
        ui->statusBar->showMessage("Video Disabled.", 3000);
    }
}
