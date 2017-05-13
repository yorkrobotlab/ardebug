/* pathsettingsdialog.cpp
 *
 * This class encapsulates a dialog for editing the name
 * visualiser display settings.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "pathsettingsdialog.h"
#include "vispath.h"
#include "settings.h"

#include <QPushButton>
#include <QFormLayout>

/* Constructor
 * Creates the dialog.
 */
PathSettingsDialog::PathSettingsDialog(VisElement* visElement) {
    // Set up the window
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("Path Visualistion Settings");

    // Create controls. Initialise to current settings
    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisPath*)visElement)->getSelectedOnly());

    posHistoryIntervalBox = new QLineEdit(QString::number(Settings::instance()->getPosHistorySampleInterval()));

    // Create buttons. Connect slots.
    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    // Lay out controls in a form
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);
    formLayout->addRow("Position sample interval:", posHistoryIntervalBox);

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
void PathSettingsDialog::accept(void) {
    // Apply settings changes
    VisPath* visPath = (VisPath*)visElement;
    visPath->setSelectedOnly(selectedOnlyCheckbox->isChecked());

    int interval = 1;
    bool ok = false;

    interval = posHistoryIntervalBox->text().toInt(&ok);

    if (ok) {
        Settings::instance()->setPosHistorySampleInterval(interval);
    }

    // Call superclass accept function
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called when user wished to cancel the changes.
 */
void PathSettingsDialog::reject(void) {
    // Call superclass reject function
    QDialog::reject();
}
