#ifndef PROXIMITYSETTINGSDIALOG_H
#define PROXIMITYSETTINGSDIALOG_H

#include "viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>

class ProximitySettingsDialog : public QDialog
{
    Q_OBJECT
public:
    ProximitySettingsDialog(VisElement* visProximity);

private:
    void accept(void);
    void reject(void);

    VisElement* visElement;

    QCheckBox* selectedOnlyCheckbox;
    QCheckBox* heatModeCheckbox;

    QLineEdit* sensorAngleBox[8];
};

#endif // PROXIMITYSETTINGSDIALOG_H
