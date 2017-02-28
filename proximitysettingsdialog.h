#ifndef PROXIMITYSETTINGSDIALOG_H
#define PROXIMITYSETTINGSDIALOG_H

#include "viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>

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
};

#endif // PROXIMITYSETTINGSDIALOG_H
