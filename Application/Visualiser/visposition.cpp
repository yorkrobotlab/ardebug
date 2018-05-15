/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot poition.
 *
 */

#include "visposition.h"
#include "../Core/settings.h"
#include <QPainter>
#include <QtMath>

#include <iostream>

/* Constructor
 * Initialise all setttings
 */
VisPosition::VisPosition(void) {
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
void VisPosition::render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect) {
    if (!isEnabled()) {
        return;
    }

    double indicatorSize = 7;

    double orientation = qDegreesToRadians(robot->getAngle()*1.0);
    double x = rect.x() + (rect.width() * robot->getPos().position.x);
    double y = rect.y() + (rect.height() * robot->getPos().position.y);
    QPointF centre = QPointF{x, y};

    auto pen = painter->pen();
    auto circlePen = pen;
    auto borderPen = pen;

    if(selected)
      circlePen.setWidth(pen.widthF() * 2);

    borderPen.setWidth(circlePen.width()+2);
    borderPen.setColor(QColor{0, 0, 0, 100});
    circlePen.setColor(robot->colour);
    painter->setPen(borderPen);
    painter->drawEllipse(centre, indicatorSize, indicatorSize);

    painter->setPen(circlePen);
    painter->drawEllipse(centre, indicatorSize, indicatorSize);

    painter->setPen(pen);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisPosition::getSettingsDialog(void) {
    return NULL;
}
