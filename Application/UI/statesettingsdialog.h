#ifndef STATESETTINGSDIALOG_H
#define STATESETTINGSDIALOG_H

#include "../Visualiser/viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>

class StateSettingsDialog : public QDialog
{
public:
    StateSettingsDialog(VisElement* visElement);

private:
    void accept(void);
    void reject(void);

    VisElement* visElement;

    QCheckBox* selectedOnlyCheckbox;
};
#endif // STATESETTINGSDIALOG_H
