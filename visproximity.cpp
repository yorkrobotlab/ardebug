#include "visproximity.h"

#include <QCheckBox>
#include <QFormLayout>

#define PI 3.14159265

VisProximity::VisProximity() {
    setType(VisType::PROXIMITY);
    setEnabled(true);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

VisProximity::~VisProximity() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

QString VisProximity::toString(void) {
    QString str = "Proximity. ";

    if (selectedOnly) {
        str = str + "\t[Selected robot only. ]";
    } else {
        str = str + "\t[All robots. ]";
    }

    return str;
}

void VisProximity::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;
    int intervalAngle = -(int)(360.0f/PROX_SENS_COUNT);

    for (int i = 0; i < PROX_SENS_COUNT; i++) {
        int raw = robot->getProximitySensorData(i) > 0 ? robot->getProximitySensorData(i) : 1;
        double len = square(((raw/4095.0) * 10.0) - 10.0);
        int a = (i * intervalAngle) + robot->getAngle();

        cv::Point end = cv::Point(x + (int)(len * cos(a * PI/180)), y + (int)(len * sin(a * PI/180)));
        line(image, cv::Point(x, y), end, robot->getColour(), 1);
    }
}

QDialog* VisProximity::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new ProximitySettingsDialog(this);
    return settingsDialog;
}

void VisProximity::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

bool VisProximity::getSelectedOnly(void) {
    return selectedOnly;
}




