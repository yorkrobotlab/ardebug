#include "proximitysettingsdialog.h"
#include "visproximity.h"

#include <QPushButton>
#include <QFormLayout>

ProximitySettingsDialog::ProximitySettingsDialog(VisElement *visElement) {
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("IR Data Visualistion Settings");

    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisProximity*)visElement)->getSelectedOnly());

    heatModeCheckbox = new QCheckBox();
    heatModeCheckbox->setChecked(((VisProximity*)visElement)->getHeatMode());

    for (int i = 0; i < 8; i++) {
        sensorAngleBox[i] = new QLineEdit(QString::number(((VisProximity*)visElement)->sensorAngles[i]));
    }

    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);
    formLayout->addRow("Show heat mode:", heatModeCheckbox);

    for (int i = 0; i < 8; i++) {
        formLayout->addRow("Sensor " + QString::number(i + 1) + " Angle:", sensorAngleBox[i]);
    }

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

    QDialog::accept();
}

void ProximitySettingsDialog::reject(void) {
    QDialog::reject();
}
