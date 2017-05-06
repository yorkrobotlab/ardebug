#include "visname.h"
#include "settings.h"

VisName::VisName(void) {
    setType(VisType::NAME);
    setEnabled(true);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

QString VisName::toString(void) {
    QString str = "Name. ";

    if (selectedOnly) {
        str = str + "\t\t[Selected robot only. ]";
    } else {
        str = str + "\t\t[All robots. ]";
    }

    return str;
}

void VisName::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    putText(image,
            robot->getName().toStdString(),
            cv::Point(x + 12, y),
            cv::FONT_HERSHEY_SIMPLEX,
            0.3,
            colour);
}

QDialog* VisName::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new NameSettingsDialog(this);
    return settingsDialog;
}

void VisName::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

bool VisName::getSelectedOnly(void) {
    return selectedOnly;
}
