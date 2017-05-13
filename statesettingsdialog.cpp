/* statesettingsdialog.cpp
 *
 * This class encapsulates a dialog for editing the state
 * visualiser display settings.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "statesettingsdialog.h"
#include "visstate.h"

#include <QPushButton>
#include <QFormLayout>

/* Constructor
 * Creates the dialog
 */
StateSettingsDialog::StateSettingsDialog(VisElement* visElement) {
    // Set up the window
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("State Visualistion Settings");

    // Create the controls. Initialise to current settings.
    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisState*)visElement)->getSelectedOnly());
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Lay out the controls in a form
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);

    // Lay out the buttons in one row
    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    // Combine the layouts vertically
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(formLayout);
    vbox->addLayout(buttonBox);

    // Set the main layout
    this->setLayout(vbox);
}

/* accept
 * Overrides QDialog accept function. Called when user wished to apply the changes.
 */
void StateSettingsDialog::accept(void) {
    // Apply settings changes
    VisState* visState = (VisState*)visElement;
    visState->setSelectedOnly(selectedOnlyCheckbox->isChecked());

    // Call superclass accept function
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called when user wished to cancel the changes.
 */
void StateSettingsDialog::reject(void) {
    // Call superclass reject function
    QDialog::reject();
}
