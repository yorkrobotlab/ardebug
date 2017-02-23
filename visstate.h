#ifndef VISSTATE_H
#define VISSTATE_H

#include "viselement.h"

class VisState : public VisElement
{
public:
    VisState();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);
};

#endif // VISSTATE_H
