/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot poition.
 *
 * (C) Alistair Jewers Feb 2017
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
void VisPosition::render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect) {
    if (!isEnabled()) {
        return;
    }

    double indicatorSize = 7;

    double orientation = qDegreesToRadians(robot->getAngle()*1.0);
    double x = rect.x() + (rect.width() * robot->getPos().position.x);
    double y = rect.y() + (rect.height() * robot->getPos().position.y);
    QPointF centre = QPointF{x, y};

    double frontDx = cos(orientation) * indicatorSize * 0.5;
    double frontDy = sin(orientation) * indicatorSize * 0.5;

    double sideDx = -sin(orientation) * indicatorSize * 0.5;
    double sideDy = cos(orientation) * indicatorSize * 0.5;

    auto pen = painter->pen();

    if(selected)
    {
        auto newPen = QPen{pen};
        newPen.setWidth(pen.widthF() * 2);
        painter->setPen(newPen);
    }

    painter->drawEllipse(centre, indicatorSize, indicatorSize);

    painter->setPen(pen);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisPosition::getSettingsDialog(void) {
    return NULL;
}
