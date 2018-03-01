/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot poition.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visposition.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisPosition::VisPosition(void) {
    setType(VisType::POSITION);
    setEnabled(true);
}

/* toString
 * Generate a string describing all settings.
 */
QString VisPosition::toString(void) {
    return QString("Position");
}

/* render
 * Render this visualisation for one robot.
 */
void VisPosition::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled()) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().position.x;
    int y = image.rows * robot->getPos().position.y;

    circle(image,
           cv::Point(x, y),
           8,
           colour,
           selected ? 2 : 1);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisPosition::getSettingsDialog(void) {
    return NULL;
}
