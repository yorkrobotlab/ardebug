#ifndef ROBOTINFODIALOG_H
#define ROBOTINFODIALOG_H

#include "robotdata.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>

class RobotInfoDialog : public QDialog
{
    Q_OBJECT
public:
    RobotInfoDialog(RobotData* robot);

public slots:
    void deleteButtonPressed(void);

signals:
    void deleteRobot(int robotID);

private:
    int ID;
    RobotData* robot;

    QLineEdit* redBox;
    QLineEdit* greenBox;
    QLineEdit* blueBox;

    void accept(void);
    void reject(void);
};

#endif // ROBOTINFODIALOG_H
