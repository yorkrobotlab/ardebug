#include "visid.h"

VisID::VisID(void) {
    setType(VisType::ID);
    setEnabled(true);
    setSelectedOnly(false);
    settingsDialog = NULL;
}

VisID::~VisID() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

QString VisID::toString(void) {
    QString str = "ID. ";

    if (selectedOnly) {
        str = str + "\t[Selected robot only. ]";
    } else {
        str = str + "\t[All robots. ]";
    }

    return str;
}

void VisID::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    putText(image, QString::number(robot->getID()).toStdString(), cv::Point(x - 24, y), cv::FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
}

QDialog* VisID::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new IDSettingsDialog(this);
    return settingsDialog;
}

void VisID::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

bool VisID::getSelectedOnly(void) {
    return selectedOnly;
}
