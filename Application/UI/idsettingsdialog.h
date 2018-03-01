#ifndef IDSETTINGSDIALOG_H
#define IDSETTINGSDIALOG_H

#include "../Visualiser/viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>

class IDSettingsDialog : public QDialog
{
public:
    IDSettingsDialog(VisElement* visElement);

private:
    void accept(void);
    void reject(void);

    VisElement* visElement;

    QCheckBox* selectedOnlyCheckbox;
};

#endif // IDSETTINGSDIALOG_H
