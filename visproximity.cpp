#include "visproximity.h"

#include <QCheckBox>
#include <QFormLayout>

#define PI 3.14159265

VisProximity::VisProximity() {
    setType(VisType::PROXIMITY);
    setEnabled(true);
    setSelectedOnly(true);
    setHeatMode(false);
    settingsDialog = NULL;
}

VisProximity::~VisProximity() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

QString VisProximity::toString(void) {
    QString str = "IR Sensors. ";

    if (selectedOnly) {
        str = str + "\t\t[Selected robot only. ]";
    } else {
        str = str + "\t\t[All robots. ]";
    }

    return str;
}

void VisProximity::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    int x = image.cols * robot->getPos().x;
    int y = image.rows * robot->getPos().y;

    for (int i = 0; i < PROX_SENS_COUNT; i++) {
        if (sensorAngles[i] < 0) {
            continue;
        }

        int raw = robot->getProximitySensorData(i) > 0 ? robot->getProximitySensorData(i) : 1;
        int a = robot->getAngle() + sensorAngles[i];

        if (heatMode) {
            int r = (int)((raw/4095.0) * 255);
            int s = (int)((raw/4095.0) * 2) + 2;
            cv::Point sens = cv::Point(x + (int)(20 * cos(a * PI/180)), y + (int)(20 * sin(a * PI/180)));

            if (raw > 140) {
                rectangle(image, sens - cv::Point(s, s), sens + cv::Point(s, s), cv::Scalar(255-r, 255-r, 255), CV_FILLED);
            } else {
                rectangle(image, sens - cv::Point(1, 1), sens + cv::Point(1, 1), cv::Scalar(115, 115, 115), CV_FILLED);
            }
        } else {
            double len = square(((raw/4095.0) * 10.0) - 10.0) * 0.4;
            cv::Point end = cv::Point(x + (int)(len * cos(a * PI/180)), y + (int)(len * sin(a * PI/180)));
            line(image, cv::Point(x, y), end, robot->getColour(), 1);
        }
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

void VisProximity::setHeatMode(bool enable) {
    heatMode = enable;
}

bool VisProximity::getHeatMode(void) {
    return heatMode;
}




