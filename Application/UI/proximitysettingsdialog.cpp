/* pathsettingsdialog.cpp
 *
 * This class encapsulates a dialog for editing the IR data
 * visualiser display settings.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "proximitysettingsdialog.h"
#include "../Visualiser/visproximity.h"

#include <QPushButton>
#include <QFormLayout>

/* Constructor.
 * Creates the dialog.
 */
ProximitySettingsDialog::ProximitySettingsDialog(VisElement *visElement) {
    // Set up the window
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("IR Data Visualistion Settings");

    // Create controls. Initialise with current settings
    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisProximity*)visElement)->getSelectedOnly());

    heatModeCheckbox = new QCheckBox();
    heatModeCheckbox->setChecked(((VisProximity*)visElement)->getHeatMode());

    for (int i = 0; i < 8; i++) {
        sensorAngleBox[i] = new QLineEdit(QString::number(((VisProximity*)visElement)->sensorAngles[i]));
    }

    // Create the buttons. Connect slots.
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Lay out the controls in a form layout
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);
    formLayout->addRow("Show heat mode:", heatModeCheckbox);

    for (int i = 0; i < 8; i++) {
        formLayout->addRow("Sensor " + QString::number(i + 1) + " Angle:", sensorAngleBox[i]);
    }

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
void ProximitySettingsDialog::accept(void) {
    // Apply settings changes
    VisProximity* visProximity = (VisProximity*)visElement;
    visProximity->setSelectedOnly(selectedOnlyCheckbox->isChecked());
    visProximity->setHeatMode(heatModeCheckbox->isChecked());

    for (int i = 0; i < 8; i++) {
        int a = 0;
        bool ok = false;

        a = sensorAngleBox[i]->text().toInt(&ok);

        if (ok) {
            visProximity->sensorAngles[i] = a;
        } else {
            visProximity->sensorAngles[i] = -1;
        }
    }

    // Call superclass accept function
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called when user wished to cancel the changes.
 */
void ProximitySettingsDialog::reject(void) {
    // Call superclass reject function
    QDialog::reject();
}
