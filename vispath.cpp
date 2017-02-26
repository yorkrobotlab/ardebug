#include "vispath.h"

VisPath::VisPath() {
    setType(VisType::PATH);
    setEnabled(true);
}

QString VisPath::toString(void) {
    return QString("Path");
}

void VisPath::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled()) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    Vector2D posHistory[POS_HISTORY_COUNT];
    robot->getPosHistory(posHistory);

    for (int i = 0; i < POS_HISTORY_COUNT; i++) {
        int ex = image.cols * posHistory[i].x;
        int ey = image.rows * posHistory[i].y;

        line(image, cv::Point(x, y), cv::Point(ex, ey), robot->getColour(), 1);

        x = ex;
        y = ey;
    }
}
