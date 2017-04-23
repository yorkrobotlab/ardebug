#include "viscustom.h"

VisCustom::VisCustom(void) {
    setType(VisType::NAME);
    setEnabled(true);
    setSelectedOnly(true);
    setTarget("None");
    settingsDialog = NULL;
}

QString VisCustom::toString(void) {
    QString str = "Custom Data. \t " + target + " ";

    if (selectedOnly) {
        str = str + "[Selected robot only. ]";
    } else {
        str = str + "[All robots. ]";
    }

    return str;
}

void VisCustom::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    QString value = robot->getCustomData(target);

    if (value != NULL) {
        putText(image, value.toStdString(), cv::Point(x + 12, y + 24), cv::FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
    }
}

QDialog* VisCustom::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new CustomSettingsDialog(this);
    return settingsDialog;
}

void VisCustom::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

bool VisCustom::getSelectedOnly(void) {
    return selectedOnly;
}

void VisCustom::setTarget(QString target) {
    this->target = target;
}

QString VisCustom::getTarget(void) {
    return target;
}
