#include "robotinfodialog.h"


#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QIntValidator>

RobotInfoDialog::RobotInfoDialog(RobotData* robot)
{
    this->ID = robot->getID();

    this->setModal(true);
    this->setGeometry(200, 200, 300, 400);
    this->setWindowTitle("Robot Info");

    QPushButton* deleteButton = new QPushButton("Delete");
    QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonPressed()));

    QFormLayout* form = new QFormLayout();
    form->addRow("ID:", new QLabel(QString::number(this->ID)));
    form->addRow("Delete this robot", deleteButton);

    QPushButton* doneButton = new QPushButton("Done");
    QObject::connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(form);
    vbox->addWidget(doneButton);

    this->setLayout(vbox);
}

void RobotInfoDialog::deleteButtonPressed(void) {
    emit deleteRobot(this->ID);
    accept();
}

void RobotInfoDialog::accept(void) {
    QDialog::accept();
}

void RobotInfoDialog::reject(void) {
    QDialog::reject();
}
