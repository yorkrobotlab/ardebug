/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot path.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "vispath.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisPath::VisPath() {
    setType(VisType::PATH);
    setEnabled(false);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisPath::~VisPath() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* toString
 * Generate a string describing all settings.
 */
QString VisPath::toString(void) {
    QString str = "Path. ";

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
void VisPath::render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected) {
//    if (!isEnabled() || (selectedOnly && !selected)) {
//        return;
//    }

//    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

//    int x = image.cols * robot->getPos().position.x;
//    int y = image.rows * robot->getPos().position.y;

//    Pose posHistory[POS_HISTORY_COUNT];
//    robot->getPosHistory(posHistory);

//    for (int i = 0; i < POS_HISTORY_COUNT; i++) {
//        int ex = image.cols * posHistory[i].position.x;
//        int ey = image.rows * posHistory[i].position.y;

//        if (x != 0 && y != 0 && ex != 0 && ey != 0) {
//            line(image,
//                 cv::Point(x, y),
//                 cv::Point(ex, ey),
//                 colour,
//                 1);
//        }

//        x = ex;
//        y = ey;
//    }
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisPath::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new PathSettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisPath::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisPath::getSelectedOnly(void) {
    return selectedOnly;
}
