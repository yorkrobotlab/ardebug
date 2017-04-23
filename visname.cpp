#include "visname.h"

VisName::VisName(void) {
    setType(VisType::NAME);
    setEnabled(true);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

QString VisName::toString(void) {
    QString str = "Name. ";

    if (selectedOnly) {
        str = str + "\t[Selected robot only. ]";
    } else {
        str = str + "\t[All robots. ]";
    }

    return str;
}

void VisName::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    putText(image, robot->getName().toStdString(), cv::Point(x + 12, y), cv::FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
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
