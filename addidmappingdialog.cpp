#include "addidmappingdialog.h"

#include "settings.h"

#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QIntValidator>

AddIDMAppingDialog::AddIDMAppingDialog(void) {
    this->setModal(true);
    this->setGeometry(200, 200, 400, 100);
    this->setWindowTitle("Add an ID mapping");

    arucoIDEdit = new QLineEdit();
    robotIDEdit = new QLineEdit();

    arucoIDEdit->setValidator(new QIntValidator(1, 100000, this));
    robotIDEdit->setValidator(new QIntValidator(1, 100000, this));

    QLabel* arucoIDLabel = new QLabel("ARuCo ID");
    QLabel* robotIDLabel = new QLabel("Robot ID");

    QPushButton* applyButton = new QPushButton("Apply");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QObject::connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(accept(void)));
    QObject::connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject(void)));

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(arucoIDLabel);
    hbox->addWidget(arucoIDEdit);
    hbox->addWidget(robotIDLabel);
    hbox->addWidget(robotIDEdit);

    QHBoxLayout* buttonBox = new QHBoxLayout();
    buttonBox->addWidget(applyButton);
    buttonBox->addWidget(cancelButton);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addLayout(buttonBox);

    this->setLayout(vbox);
}

void AddIDMAppingDialog::accept(void) {
    bool ok = false;

    ArucoIDPair idPair;

    idPair.arucoID = arucoIDEdit->text().toInt(&ok);

    if (!ok) {
        QDialog::reject();
        return;
    }

    idPair.robotID = robotIDEdit->text().toInt(&ok);

    if (!ok) {
        QDialog::reject();
        return;
    }

    for (size_t i = 0; i < Settings::instance()->idMapping.size(); i++) {
        if (idPair.arucoID == Settings::instance()->idMapping.at(i)->arucoID ||
            idPair.robotID == Settings::instance()->idMapping.at(i)->robotID) {
            QDialog::reject();
            return;
        }
    }

    ArucoIDPair* newPair = new ArucoIDPair;
    newPair->arucoID = idPair.arucoID;
    newPair->robotID = idPair.robotID;
    Settings::instance()->idMapping.push_back(newPair);

    QDialog::accept();
}

void AddIDMAppingDialog::reject(void) {
    QDialog::reject();
}
