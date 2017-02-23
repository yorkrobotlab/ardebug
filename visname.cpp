#include "visname.h"

VisName::VisName(void) {
    setType(VisType::NAME);
}

QString VisName::toString(void) {
    return QString("Show robot name.");
}

void VisName::render(cv::Mat image, RobotData *robot, bool selected) {
    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    if (selected) {
        putText(image, robot->getName().toStdString(), cv::Point(x + 8, y), cv::FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
    }
}
