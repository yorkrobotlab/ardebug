#ifndef VISELEMENT_H
#define VISELEMENT_H

#include <QString>

#include <opencv2/opencv.hpp>

#include "robotdata.h"

enum VisType { NAME, STATE, POSITION, DIRECTION, PROXIMITY };

class VisElement
{
public:
    VisType type;

    virtual QString toString(void) = 0;

    virtual void render(cv::Mat image, RobotData* robot, bool selected) = 0;
};

#endif // VISELEMENT_H
