/* idsettingsdialog.cpp
 *
 * This class encapsulates a dialog for editing the id
 * visualiser display settings.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "idsettingsdialog.h"
#include "../Visualiser/visid.h"

#include <QPushButton>
#include <QFormLayout>

/* Constructor
 * Creates the dialog.
 */
IDSettingsDialog::IDSettingsDialog(VisElement* visElement) {
    // Window set up
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("ID Visualistion Settings");

    // Create controls. Initialise to match current settings. Connect slots.
    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisID*)visElement)->getSelectedOnly());
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Lay out settings controls in a form layout
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);

    // Lay out buttons on one row
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
void IDSettingsDialog::accept(void) {
    // Apply settings changes.
    VisID* visID = (VisID*)visElement;
    visID->setSelectedOnly(selectedOnlyCheckbox->isChecked());

    // Call superclass accept function
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called when user wished to cancel the changes.
 */
void IDSettingsDialog::reject(void) {
    // Call superclass reject function
    QDialog::reject();
}
