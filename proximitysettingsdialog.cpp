#include "proximitysettingsdialog.h"
#include "visproximity.h"

#include <QPushButton>
#include <QFormLayout>

ProximitySettingsDialog::ProximitySettingsDialog(VisElement *visElement) {
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("Proximity Visualistion Settings");

    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisProximity*)visElement)->getSelectedOnly());

    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);

    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(formLayout);
    vbox->addLayout(buttonBox);

    this->setLayout(vbox);
}

void ProximitySettingsDialog::accept(void) {
    VisProximity* visProximity = (VisProximity*)visElement;
    visProximity->setSelectedOnly(selectedOnlyCheckbox->isChecked());

    QDialog::accept();
}

void ProximitySettingsDialog::reject(void) {
    QDialog::reject();
}
