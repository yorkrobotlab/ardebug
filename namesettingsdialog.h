#ifndef NAMESETTINGSDIALOG_H
#define NAMESETTINGSDIALOG_H

#include "viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>

class NameSettingsDialog : public QDialog
{
public:
    NameSettingsDialog(VisElement* visElement);

private:
    void accept(void);
    void reject(void);

    VisElement* visElement;

    QCheckBox* selectedOnlyCheckbox;
};

#endif // NAMESETTINGSDIALOG_H
