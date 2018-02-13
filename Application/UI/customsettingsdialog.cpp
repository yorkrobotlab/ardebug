/* customsettingsdialog.cpp
 *
 * This class encapsulates a dialog for editing the custom data
 * visualiser display settings.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "customsettingsdialog.h"
#include "../Visualiser/viscustom.h"

#include <QPushButton>
#include <QFormLayout>

/* Constructor
 * Creates the dialog.
 */
CustomSettingsDialog::CustomSettingsDialog(VisElement* visElement) {
    // Window set up
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("Custom Data Visualisation Settings");

    // Create controls and initialise to match the current settings
    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisCustom*)visElement)->getSelectedOnly());

    targetBox = new QLineEdit(((VisCustom*)visElement)->getTarget());

    // Create and attach apply and cancel buttons
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Lay out controls in a form layout
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);
    formLayout->addRow("Target data point:", targetBox);

    // Layout buttons in one row
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
void CustomSettingsDialog::accept(void) {
    // Apply settings changes
    VisCustom* visCustom = (VisCustom*)visElement;
    visCustom->setSelectedOnly(selectedOnlyCheckbox->isChecked());
    visCustom->setTarget(targetBox->text());

    // Call superclass accept function
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called when user wished to cancel the changes.
 */
void CustomSettingsDialog::reject(void) {
    QDialog::reject();
}
