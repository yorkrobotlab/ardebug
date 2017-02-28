#include "visposition.h"

VisPosition::VisPosition(void) {
    setType(VisType::POSITION);
    setEnabled(true);
}

QString VisPosition::toString(void) {
    return QString("Position");
}

void VisPosition::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled()) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    circle(image, cv::Point(x, y), 8, robot->getColour(), selected ? 2 : 1);
}

QDialog* VisPosition::getSettingsDialog(void) {
    return NULL;
}
