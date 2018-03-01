/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot id.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visid.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisID::VisID(void) {
    setType(VisType::ID);
    setEnabled(true);
    setSelectedOnly(false);
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisID::~VisID() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* toString
 * Generate a string describing all settings.
 */
QString VisID::toString(void) {
    QString str = "ID. ";

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
void VisID::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled() || (selectedOnly && !selected)) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().position.x;
    int y = image.rows * robot->getPos().position.y;

    cv::putText(image,
            robot->getID().toStdString(),
            cv::Point(x - 24, y),
            cv::FONT_HERSHEY_SIMPLEX,
            0.3,
            colour);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisID::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new IDSettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisID::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisID::getSelectedOnly(void) {
    return selectedOnly;
}
