/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot poition.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visposition.h"
#include "../Core/settings.h"
#include <QPainter>

#include <iostream>

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
void VisPosition::render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected) {
    if (!isEnabled()) {
        return;
    }

    std::cout<<widget->width() << "|" << widget->height() <<std::endl;

    double x = widget->width() * robot->getPos().position.x;
    double y = widget->height() * robot->getPos().position.y;

    auto pen = painter->pen();

    if(selected)
    {
        auto newPen = QPen{pen};
        newPen.setWidth(pen.widthF() * 2);
        painter->setPen(newPen);
    }

    painter->drawEllipse(QPointF{x, y}, 10, 10);

    painter->setPen(pen);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisPosition::getSettingsDialog(void) {
    return NULL;
}
