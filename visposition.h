#ifndef VISPOSITION_H
#define VISPOSITION_H

#include "viselement.h"

class VisPosition : public VisElement
{
public:
    VisPosition(void);

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);
};

#endif // VISPOSITION_H
