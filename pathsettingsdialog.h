#ifndef PATHSETTINGSDIALOG_H
#define PATHSETTINGSDIALOG_H

#include "viselement.h"

#include <QObject>
#include <QDialog>
#include <QCheckBox>

class PathSettingsDialog : public QDialog
{
public:
    PathSettingsDialog(VisElement* visElement);

private:
    void accept(void);
    void reject(void);

    VisElement* visElement;

    QCheckBox* selectedOnlyCheckbox;
};

#endif // PATHSETTINGSDIALOG_H
