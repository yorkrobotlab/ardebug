#ifndef VISCUSTOM_H
#define VISCUSTOM_H

#include "viselement.h"
#include "customsettingsdialog.h"

#include <QString>

class VisCustom : public VisElement
{
public:
    VisCustom();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);

    virtual QDialog* getSettingsDialog(void);

    void setSelectedOnly(bool enable);
    bool getSelectedOnly(void);

    void setTarget(QString target);
    QString getTarget(void);

private:
    CustomSettingsDialog* settingsDialog;

    bool selectedOnly;
    QString target;
};

#endif // VISCUSTOM_H
