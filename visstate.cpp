#include "visstate.h"

VisState::VisState() {
    setType(VisType::STATE);
}

QString VisState::toString(void) {
    return QString("Show robot state.");
}


void VisState::render(cv::Mat image, RobotData *robot, bool selected) {
    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    if (selected) {
        putText(image, robot->getState().toStdString(), cv::Point(x + 8, y + 12), cv::FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
    }
}
