#include "robotinfodialog.h"

#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QIntValidator>

RobotInfoDialog::RobotInfoDialog(RobotData* robot)
{
    this->ID = robot->getID();
    this->robot = robot;

    this->setModal(true);
    this->setGeometry(200, 200, 300, 400);
    this->setWindowTitle("Robot Info");

    QPushButton* deleteButton = new QPushButton("Delete");
    QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonPressed()));

    redBox = new QLineEdit(QString::number(robot->getColour()[2]));
    greenBox = new QLineEdit(QString::number(robot->getColour()[1]));
    blueBox = new QLineEdit(QString::number(robot->getColour()[0]));

    QIntValidator* colourValidator = new QIntValidator(0, 255, this);
    redBox->setValidator(colourValidator);
    greenBox->setValidator(colourValidator);
    blueBox->setValidator(colourValidator);

    QLabel* r = new QLabel("R:");
    QLabel* g = new QLabel("G:");
    QLabel* b = new QLabel("B:");

    QHBoxLayout* colour = new QHBoxLayout();
    colour->addWidget(r);
    colour->addWidget(redBox);
    colour->addWidget(g);
    colour->addWidget(greenBox);
    colour->addWidget(b);
    colour->addWidget(blueBox);

    QFormLayout* form = new QFormLayout();
    form->addRow("ID:", new QLabel(QString::number(this->ID)));
    form->addRow("Colour:", colour);
    form->addRow("Delete this robot", deleteButton);

    QPushButton* doneButton = new QPushButton("Done");
    QObject::connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(accept()));

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(form);
    vbox->addWidget(doneButton);

    this->setLayout(vbox);
}

void RobotInfoDialog::deleteButtonPressed(void) {
    emit deleteRobot(this->ID);
    reject();
}

void RobotInfoDialog::accept(void) {
    int r = redBox->text().toInt();
    int g = greenBox->text().toInt();
    int b = blueBox->text().toInt();

    robot->setColour(cv::Scalar(b, g, r));

    QDialog::accept();
}

void RobotInfoDialog::reject(void) {
    QDialog::reject();
}
