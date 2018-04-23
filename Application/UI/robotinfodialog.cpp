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
#include <QColorDialog>


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

    // Create delete button, and connect to slot
    QPushButton* colourButton = new QPushButton("Choose Colour");
    QObject::connect(colourButton, SIGNAL(clicked(bool)), this, SLOT(setColour()));

    // Create colour label and initilise it with current colour
    colourLabel = new QLabel("Colour");
    colourLabel->setText(colour.name());
    colourLabel->setPalette(QPalette(colour));
    colourLabel->setAutoFillBackground(true);

    // Layout the colour controls in a row
    QHBoxLayout* colourLayout = new QHBoxLayout();
    colourLayout->addWidget(colourLabel);
    colourLayout->addWidget(colourButton);

    // Layout all controls in a form
    QFormLayout* form = new QFormLayout();
    form->addRow("ID:", new QLabel(this->ID));
    form->addRow("Colour:", colourLayout);
    form->addRow("Delete this robot", deleteButton);

    // Create the done button and connect it
    QPushButton* doneButton = new QPushButton("Done");    
    QObject::connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(accept()));


    // Combine all layouts vertically
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(form);
    vbox->addWidget(doneButton);

    //make sure deleteButton is not pressed when pressing enter
    deleteButton->setAutoDefault(false);

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

void RobotInfoDialog::setColour()
{

    QColor new_colour = QColorDialog::getColor(colour, this, "Select Colour");

    if (new_colour.isValid()) {
        colourLabel->setText(new_colour.name());
        colourLabel->setPalette(QPalette(new_colour));
        colourLabel->setAutoFillBackground(true);
        colour.setRgb( new_colour.rgb());
    }
}
