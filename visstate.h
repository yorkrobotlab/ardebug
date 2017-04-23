#ifndef VISSTATE_H
#define VISSTATE_H

#include "viselement.h"
#include "statesettingsdialog.h"

class VisState : public VisElement
{
public:
    VisState();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);

    virtual QDialog* getSettingsDialog(void);

    void setSelectedOnly(bool enable);
    bool getSelectedOnly(void);

private:
    StateSettingsDialog* settingsDialog;

    bool selectedOnly;
};

#endif // VISSTATE_H
