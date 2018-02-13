#ifndef CUSTOMSETTINGSDIALOG_H
#define CUSTOMSETTINGSDIALOG_H

#include "../Visualiser/viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>

class CustomSettingsDialog : public QDialog
{
public:
    CustomSettingsDialog(VisElement* visElement);

private:
    void accept(void);
    void reject(void);

    VisElement* visElement;

    QCheckBox* selectedOnlyCheckbox;
    QLineEdit* targetBox;
};

#endif // CUSTOMSETTINGSDIALOG_H
