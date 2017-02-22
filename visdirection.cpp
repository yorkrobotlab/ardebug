#include "visdirection.h"

#define PI 3.14159265

VisDirection::VisDirection(void) {
    this->type = VisType::DIRECTION;
}

QString VisDirection::toString(void) {
    return QString("Direction Visualisation.");
}

void VisDirection::render(cv::Mat image, RobotData *robot, bool selected) {
    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;
    int a = robot->getAngle();


    cv::Point end = cv::Point(x + (int)(12 * cos(a * PI/180)), y + (int)(12 * sin(a * PI/180)));
    line(image, cv::Point(x, y), end, robot->getColour(), selected ? 2 : 1);
}
