#ifndef VISPROXIMITY_H
#define VISPROXIMITY_H

#include "viselement.h"

class VisProximity : public VisElement
{
public:
    VisProximity();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);
};

#endif // VISPROXIMITY_H
