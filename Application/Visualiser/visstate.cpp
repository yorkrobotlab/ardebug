/* viscustom.cpp
 *
 * This class encapsulates the visualisation of state data.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visstate.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisState::VisState() {
    setType(VisType::STATE);
    setEnabled(true);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

/* toString
 * Generate a string describing all settings.
 */
QString VisState::toString(void) {
    QString str = "State. ";

    if (selectedOnly) {
        str = str + "\t\t[Selected robot only. ]";
    } else {
        str = str + "\t\t[All robots. ]";
    }

    return str;
}

/* render
 * Render this visualisation for one robot.
 */
void VisState::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().position.x;
    int y = image.rows * robot->getPos().position.y;

    putText(image,
            robot->getState().toStdString(),
            cv::Point(x + 12, y + 12),
            cv::FONT_HERSHEY_SIMPLEX,
            0.3,
            colour);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisState::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new StateSettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisState::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisState::getSelectedOnly(void) {
    return selectedOnly;
}
