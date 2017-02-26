#ifndef VISPATH_H
#define VISPATH_H

#include "viselement.h"

class VisPath : public VisElement
{
public:
    VisPath();

    virtual QString toString(void);

    virtual void render(cv::Mat image, RobotData *robot, bool selected);
};

#endif // VISPATH_H
