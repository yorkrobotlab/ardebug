/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot direction.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visdirection.h"
#include "../Core/settings.h"

#define PI 3.14159265

/* Constructor
 * Initialise all setttings
 */
VisDirection::VisDirection(void) {
    setType(VisType::DIRECTION);
    setEnabled(true);
}

/* toString
 * Generate a string describing all settings.
 */
QString VisDirection::toString(void) {
    return QString("Direction");
}

/* render
 * Render this visualisation for one robot.
 */
void VisDirection::render(cv::Mat image, RobotData *robot, bool selected) {
    if (!isEnabled()) {
        return;
    }

    cv::Scalar colour = Settings::instance()->isRobotColourEnabled() ? robot->getColour() : cv::Scalar(255, 255, 255);

    int x = image.cols * robot->getPos().position.x;
    int y = image.rows * robot->getPos().position.y;
    int a = robot->getAngle();

    cv::Point end = cv::Point(x + (int)(12 * cos(a * PI/180)), y + (int)(12 * sin(a * PI/180)));

    line(image,
         cv::Point(x, y),
         end,
         colour,
         selected ? 2 : 1);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisDirection::getSettingsDialog(void) {
    return NULL;
}
