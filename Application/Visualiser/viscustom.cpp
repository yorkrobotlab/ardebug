/* viscustom.cpp
 *
 * This class encapsulates the visualisation of custom data.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "viscustom.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisCustom::VisCustom(void) {
    setType(VisType::CUSTOM);
    setEnabled(false);
    setSelectedOnly(true);
    setTarget("None");
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisCustom::~VisCustom() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* toString
 * Generate a string describing all settings.
 */
QString VisCustom::toString(void) {
    QString str = "Custom Data.";

    if (selectedOnly) {
        str = str + "\t[Selected robot only. Target: " + target + "]";
    } else {
        str = str + "\t[All robots. Target: " + target + "]";
    }

    return str;
}

/* render
 * Render this visualisation for one robot.
 */
void VisCustom::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().position.x;
    int y = image.rows * robot->getPos().position.y;

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

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisCustom::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new CustomSettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisCustom::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisCustom::getSelectedOnly(void) {
    return selectedOnly;
}

/* setTarget
 * Set the target custom data point by key
 */
void VisCustom::setTarget(QString target) {
    this->target = target;
}

/* getTarget
 * Get the key of the target custom data point
 */
QString VisCustom::getTarget(void) {
    return target;
}
