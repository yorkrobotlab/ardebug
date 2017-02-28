#include "vispath.h"

VisPath::VisPath() {
    setType(VisType::PATH);
    setEnabled(true);
    selectedOnly = false;
    settingsDialog = NULL;
}

VisPath::~VisPath() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

QString VisPath::toString(void) {
    QString str = "Path. ";

    if (selectedOnly) {
        str = str + "\t[Selected robot only. ]";
    } else {
        str = str + "\t[All robots. ]";
    }

    return str;
}

void VisPath::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
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

QDialog* VisPath::getSettingsDialog(void) {
    settingsDialog = new PathSettingsDialog(this);
    return settingsDialog;
}

void VisPath::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

bool VisPath::getSelectedOnly(void) {
    return selectedOnly;
}
