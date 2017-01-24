#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_testButton_clicked();

    void on_actionExit_triggered();

    void on_actionEnable_Video_changed();

    void on_videoEnChbx_stateChanged();

private:
    Ui::MainWindow *ui;

    void setVideo(bool enabled);
};

#endif // MAINWINDOW_H
