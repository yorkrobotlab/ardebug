#include "visproximity.h"

#define PI 3.14159265

VisProximity::VisProximity() {
    setType(VisType::PROXIMITY);
    setEnabled(true);
}

QString VisProximity::toString(void) {
    return QString("Show proximity sensor data.");
}

void VisProximity::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || !selected) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;
    int intervalAngle = (int)(360.0f/PROX_SENS_COUNT);

    for (int i = 0; i < PROX_SENS_COUNT; i++) {
        int a = (i * intervalAngle) + robot->getAngle();
        int len = (4096 - robot->getProximitySensorData(i))/50;

        cv::Point end = cv::Point(x + (int)(len * cos(a * PI/180)), y + (int)(len * sin(a * PI/180)));
        line(image, cv::Point(x, y), end, robot->getColour(), 1);
    }
}
