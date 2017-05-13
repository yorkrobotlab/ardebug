/* robotinfodialog.cpp
 *
 * This class encapsulates a small dialog for showing info about a specific
 * robot, including its display colour. Also includes a control for deleting
 * the robot from the data model.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "robotinfodialog.h"

#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QIntValidator>

/* Constructor
 * Creates the dialog
 */
RobotInfoDialog::RobotInfoDialog(RobotData* robot)
{
    // Get robot data
    this->ID = robot->getID();
    this->robot = robot;

    // Set up window
    this->setModal(true);
    this->setGeometry(200, 200, 300, 400);
    this->setWindowTitle("Robot Info");

    // Create delete button, and connect to slot
    QPushButton* deleteButton = new QPushButton("Delete");
    QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonPressed()));

    // Create colour controls
    redBox = new QLineEdit(QString::number(robot->getColour()[2]));
    greenBox = new QLineEdit(QString::number(robot->getColour()[1]));
    blueBox = new QLineEdit(QString::number(robot->getColour()[0]));

    // Validate colour control entries to integers, 0-255
    QIntValidator* colourValidator = new QIntValidator(0, 255, this);
    redBox->setValidator(colourValidator);
    greenBox->setValidator(colourValidator);
    blueBox->setValidator(colourValidator);

    // Create colour labels
    QLabel* r = new QLabel("R:");
    QLabel* g = new QLabel("G:");
    QLabel* b = new QLabel("B:");

    // Layout the colour controls in a row
    QHBoxLayout* colour = new QHBoxLayout();
    colour->addWidget(r);
    colour->addWidget(redBox);
    colour->addWidget(g);
    colour->addWidget(greenBox);
    colour->addWidget(b);
    colour->addWidget(blueBox);

    // Layout all controls in a form
    QFormLayout* form = new QFormLayout();
    form->addRow("ID:", new QLabel(QString::number(this->ID)));
    form->addRow("Colour:", colour);
    form->addRow("Delete this robot", deleteButton);

    // Create the done button and connect it
    QPushButton* doneButton = new QPushButton("Done");
    QObject::connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(accept()));

    // Combine all layouts vertically
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(form);
    vbox->addWidget(doneButton);

    // Set main layout
    this->setLayout(vbox);
}

/* deleteButtonPressed
 * Called if the user preses the delete button.
 */
void RobotInfoDialog::deleteButtonPressed(void) {
    // Emit the signal to trigger robot data deletion
    emit deleteRobot(this->ID);

    // Reject the dialog
    reject();
}

/* accept
 * Overrides QDialog accept function. Called when user wished to apply the changes.
 */
void RobotInfoDialog::accept(void) {
    // Apply changes
    int r = redBox->text().toInt();
    int g = greenBox->text().toInt();
    int b = blueBox->text().toInt();

    robot->setColour(cv::Scalar(b, g, r));

    // Call superclass accept function
    QDialog::accept();
}

/* accept
 * Overrides QDialog reject function. In this case only called when delete button is
 * pressed, to close the dialog.
 */
void RobotInfoDialog::reject(void) {
    // Call the superclass reject function
    QDialog::reject();
}
