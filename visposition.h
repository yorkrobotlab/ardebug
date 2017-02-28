#ifndef VISPOSITION_H
#define VISPOSITION_H

#include "viselement.h"

class VisPosition : public VisElement
{
public:
    VisPosition(void);

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);

    virtual QDialog* getSettingsDialog(void);
};

#endif // VISPOSITION_H
