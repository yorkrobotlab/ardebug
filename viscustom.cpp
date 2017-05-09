#include "viscustom.h"
#include "settings.h"

VisCustom::VisCustom(void) {
    setType(VisType::CUSTOM);
    setEnabled(false);
    setSelectedOnly(true);
    setTarget("None");
    settingsDialog = NULL;
}

QString VisCustom::toString(void) {
    QString str = "Custom Data.";

    if (selectedOnly) {
        str = str + "\t[Selected robot only. Target: " + target + "]";
    } else {
        str = str + "\t[All robots. Target: " + target + "]";
    }

    return str;
}

void VisCustom::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    QString value = robot->getCustomData(target);

    if (!value.isEmpty()) {
        value = target + ": " + value;
    }

    if (value != NULL) {
        putText(image,
                value.toStdString(),
                cv::Point(x + 12, y + 24),
                cv::FONT_HERSHEY_SIMPLEX, 0.3,
                colour);
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
