#include "visstate.h"

VisState::VisState() {
    setType(VisType::STATE);
    setEnabled(true);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

QString VisState::toString(void) {
    QString str = "State. ";

    if (selectedOnly) {
        str = str + "\t\t[Selected robot only. ]";
    } else {
        str = str + "\t\t[All robots. ]";
    }

    return str;
}


void VisState::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    putText(image, robot->getState().toStdString(), cv::Point(x + 12, y + 12), cv::FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
}

QDialog* VisState::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new StateSettingsDialog(this);
    return settingsDialog;
}

void VisState::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

bool VisState::getSelectedOnly(void) {
    return selectedOnly;
}
