#include "visposition.h"

VisPosition::VisPosition(void) {
    this->type = VisType::POSITION;
}

QString VisPosition::toString(void) {
    return QString("Position Visualisation.");
}

void VisPosition::render(cv::Mat image, RobotData *robot, bool selected) {
    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    circle(image, cv::Point(x, y), 8, robot->getColour(), selected ? 2 : 1);
}
