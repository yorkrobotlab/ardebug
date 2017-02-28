#ifndef VISPROXIMITY_H
#define VISPROXIMITY_H

#include "viselement.h"
#include "proximitysettingsdialog.h"

#include <QDialog>

class VisProximity : public VisElement
{
public:
    VisProximity();
    ~VisProximity();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);

    virtual QDialog* getSettingsDialog(void);

    void setSelectedOnly(bool enable);
    bool getSelectedOnly(void);

private:
    ProximitySettingsDialog* settingsDialog;

    // Settings
    bool selectedOnly;
};

#endif // VISPROXIMITY_H
