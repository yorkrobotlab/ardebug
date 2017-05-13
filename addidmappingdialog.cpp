/* addidmappingdialog.cpp
 *
 * This class encapsulates a small dialog for adding a new tag ID -> robot ID mapping.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "addidmappingdialog.h"

#include "settings.h"

#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QIntValidator>

/* Constructor
 * Construct the dialog. Connect controls to slots.
 */
AddIDMAppingDialog::AddIDMAppingDialog(void) {
    // Set up window
    this->setModal(true);
    this->setGeometry(200, 200, 400, 100);
    this->setWindowTitle("Add an ID mapping");

    // Text boxes for entering ID numbers
    arucoIDEdit = new QLineEdit();
    robotIDEdit = new QLineEdit();

    // Limit to positive integers or zero
    arucoIDEdit->setValidator(new QIntValidator(0, 100000, this));
    robotIDEdit->setValidator(new QIntValidator(0, 100000, this));

    // Labels
    QLabel* arucoIDLabel = new QLabel("ARuCo ID");
    QLabel* robotIDLabel = new QLabel("Robot ID");

    // Buttons to cancel or apply
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Layout the boxes and labels on one row
    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(arucoIDLabel);
    hbox->addWidget(arucoIDEdit);
    hbox->addWidget(robotIDLabel);
    hbox->addWidget(robotIDEdit);

    // Layout the buttons on another row
    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    // Arrange the rows vertically
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addLayout(buttonBox);

    // Set layout
    this->setLayout(vbox);
}

/* accept
 * Overrides the accept function of the QDialog. Called if the user wishes
 * to apply the changes within the dialog.
 */
void AddIDMAppingDialog::accept(void) {
    bool ok = false;
    ArucoIDPair idPair;

    // Parse the AruCo ID
    idPair.arucoID = arucoIDEdit->text().toInt(&ok);

    if (!ok) {
        QDialog::reject();
        return;
    }

    // Parse the robot ID
    idPair.robotID = robotIDEdit->text().toInt(&ok);

    if (!ok) {
        QDialog::reject();
        return;
    }

    // Check for existing matches for either ID
    for (size_t i = 0; i < Settings::instance()->idMapping.size(); i++) {
        if (idPair.arucoID == Settings::instance()->idMapping.at(i)->arucoID ||
            idPair.robotID == Settings::instance()->idMapping.at(i)->robotID) {
            QDialog::reject();
            return;
        }
    }

    // Allocate memory for new pair object and fill it, then add to mapping list
    ArucoIDPair* newPair = new ArucoIDPair;
    newPair->arucoID = idPair.arucoID;
    newPair->robotID = idPair.robotID;
    Settings::instance()->idMapping.push_back(newPair);

    // Call superclass function to accept and close the dialog
    QDialog::accept();
}

/* reject
 * Overrides the reject function of the QDialog. Called if the user wishes
 * to cancel the changes within the dialog.
 */
void AddIDMAppingDialog::reject(void) {
    QDialog::reject();
}
