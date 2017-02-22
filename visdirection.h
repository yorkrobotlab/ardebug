#ifndef VISDIRECTION_H
#define VISDIRECTION_H

#include "viselement.h"

class VisDirection : public VisElement
{
public:
    VisDirection(void);

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);
};

#endif // VISDIRECTION_H
