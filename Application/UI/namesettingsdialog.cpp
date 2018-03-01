/* namesettingsdialog.cpp
 *
 * This class encapsulates a dialog for editing the name
 * visualiser display settings.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "namesettingsdialog.h"
#include "../Visualiser/visname.h"

#include <QPushButton>
#include <QFormLayout>

/* Constructor
 * Creates the dialog.
 */
NameSettingsDialog::NameSettingsDialog(VisElement* visElement) {
    // Set up the window
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("Name Visualistion Settings");

    // Create controls. Initialise with current settings.
    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisName*)visElement)->getSelectedOnly());

    // Create apply and cancel buttons. Connect slots.
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Lay out settings controls in a form layout
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);

    // Lay out buttons in one row
    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    // Combine layouts vertically
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(formLayout);
    vbox->addLayout(buttonBox);

    // Set main layout
    this->setLayout(vbox);
}

/* accept
 * Overrides QDialog accept function. Called when user wished to apply the changes.
 */
void NameSettingsDialog::accept(void) {
    // Apply settings changes
    VisName* visName = (VisName*)visElement;
    visName->setSelectedOnly(selectedOnlyCheckbox->isChecked());

    // Call superclas accept function
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called when user wished to cancel the changes.
 */
void NameSettingsDialog::reject(void) {
    // Call superclass reject function
    QDialog::reject();
}
