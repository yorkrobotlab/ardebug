#include "visdirection.h"
#include "settings.h"

#define PI 3.14159265

VisDirection::VisDirection(void) {
    setType(VisType::DIRECTION);
    setEnabled(true);
}

QString VisDirection::toString(void) {
    return QString("Direction");
}

void VisDirection::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled()) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;
    int a = robot->getAngle();

    cv::Point end = cv::Point(x + (int)(12 * cos(a * PI/180)), y + (int)(12 * sin(a * PI/180)));

    line(image,
         cv::Point(x, y),
         end,
         colour,
         selected ? 2 : 1);
}

QDialog* VisDirection::getSettingsDialog(void) {
    return NULL;
}
