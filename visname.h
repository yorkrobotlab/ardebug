#ifndef VISNAME_H
#define VISNAME_H

#include "viselement.h"

class VisName : public VisElement
{
public:
    VisName();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);

    virtual QDialog* getSettingsDialog(void);
};

#endif // VISNAME_H
