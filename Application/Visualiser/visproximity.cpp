/* viscustom.cpp
 *
 * This class encapsulates the visualisation of IR data.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visproximity.h"
#include "../Core/settings.h"

#define PI 3.14159265

/* Constructor
 * Initialise all setttings
 */
VisProximity::VisProximity() {
    setType(VisType::PROXIMITY);
    setEnabled(false);
    setSelectedOnly(true);
    setHeatMode(false);
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisProximity::~VisProximity() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* toString
 * Generate a string describing all settings.
 */
QString VisProximity::toString(void) {
    QString str = "IR Sensors. ";

    if (selectedOnly) {
        str = str + "\t\t[Selected robot only. ";
    } else {
        str = str + "\t\t[All robots. ";
    }

    if (heatMode) {
        str = str + "Heat Mode. Angles:";
    } else {
        str = str + "Proximity Mode. Angles:";
    }

    for (int i = 0; i < 8; i++) {
        str = str + " " + QString::number(sensorAngles[i]);
    }

    str = str + "]";

    return str;
}

/* render
 * Render this visualisation for one robot.
 */
void VisProximity::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

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
                rectangle(image,
                          sens - cv::Point(s, s),
                          sens + cv::Point(s, s),
                          cv::Scalar(255-r, 255-r, 255),
                          CV_FILLED);
            } else {
                rectangle(image,
                          sens - cv::Point(1, 1),
                          sens + cv::Point(1, 1),
                          cv::Scalar(115, 115, 115),
                          CV_FILLED);
            }
        } else {
            double len = square(((raw/4095.0) * 10.0) - 10.0) * 0.4;
            cv::Point end = cv::Point(x + (int)(len * cos(a * PI/180)), y + (int)(len * sin(a * PI/180)));
            line(image,
                 cv::Point(x, y),
                 end,
                 colour,
                 1);
        }
    }
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisProximity::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new ProximitySettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisProximity::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisProximity::getSelectedOnly(void) {
    return selectedOnly;
}

/* setHeatMode
 * Enable or disable heat mode display
 */
void VisProximity::setHeatMode(bool enable) {
    heatMode = enable;
}

/* getHeatMode
 * Get the display mode setting
 */
bool VisProximity::getHeatMode(void) {
    return heatMode;
}




