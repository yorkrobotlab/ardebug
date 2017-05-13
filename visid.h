#ifndef VISID_H
#define VISID_H

#include "viselement.h"
#include "idsettingsdialog.h"

class VisID : public VisElement
{
public:
    VisID();
    ~VisID();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);

    virtual QDialog* getSettingsDialog(void);

    void setSelectedOnly(bool enable);
    bool getSelectedOnly(void);

private:
    IDSettingsDialog* settingsDialog;

    bool selectedOnly;
};

#endif // VISID_H
