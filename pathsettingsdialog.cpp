#include "pathsettingsdialog.h"
#include "vispath.h"
#include "settings.h"

#include <QPushButton>
#include <QFormLayout>

PathSettingsDialog::PathSettingsDialog(VisElement* visElement) {
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("Path Visualistion Settings");

    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisPath*)visElement)->getSelectedOnly());

    posHistoryIntervalBox = new QLineEdit(QString::number(Settings::instance()->getPosHistorySampleInterval()));

    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);
    formLayout->addRow("Position sample interval:", posHistoryIntervalBox);

    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(formLayout);
    vbox->addLayout(buttonBox);

    this->setLayout(vbox);
}

void PathSettingsDialog::accept(void) {
    VisPath* visPath = (VisPath*)visElement;
    visPath->setSelectedOnly(selectedOnlyCheckbox->isChecked());

    int interval = 1;
    bool ok = false;

    interval = posHistoryIntervalBox->text().toInt(&ok);

    if (ok) {
        Settings::instance()->setPosHistorySampleInterval(interval);
    }

    QDialog::accept();
}

void PathSettingsDialog::reject(void) {
    QDialog::reject();
}
