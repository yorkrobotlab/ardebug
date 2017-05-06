#include "customsettingsdialog.h"
#include "viscustom.h"

#include <QPushButton>
#include <QFormLayout>

CustomSettingsDialog::CustomSettingsDialog(VisElement* visElement) {
    this->visElement = visElement;
    this->setModal(true);
    this->setGeometry(300, 300, 400, 400);
    this->setWindowTitle("Custom Data Visualisation Settings");

    selectedOnlyCheckbox = new QCheckBox();
    selectedOnlyCheckbox->setChecked(((VisCustom*)visElement)->getSelectedOnly());

    targetBox = new QLineEdit(((VisCustom*)visElement)->getTarget());

    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Show only for selected:", selectedOnlyCheckbox);
    formLayout->addRow("Target data point:", targetBox);

    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(formLayout);
    vbox->addLayout(buttonBox);

    this->setLayout(vbox);
}

void CustomSettingsDialog::accept(void) {
    VisCustom* visCustom = (VisCustom*)visElement;
    visCustom->setSelectedOnly(selectedOnlyCheckbox->isChecked());
    visCustom->setTarget(targetBox->text());

    QDialog::accept();
}

void CustomSettingsDialog::reject(void) {
    QDialog::reject();
}
