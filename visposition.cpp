#include "visposition.h"
#include "settings.h"

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

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    circle(image,
           cv::Point(x, y),
           8,
           colour,
           selected ? 2 : 1);
}

QDialog* VisPosition::getSettingsDialog(void) {
    return NULL;
}
