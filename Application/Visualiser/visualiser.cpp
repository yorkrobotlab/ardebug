/* visualiser.cpp
 *
 * This class encapsulates the visualiser Qt widget for displaying tracking data
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visualiser.h"
#include "../Core/settings.h"

#include <stdio.h>
#include <math.h>
#include <cmath>

#include <iostream>
#include <QLayout>

#include "visid.h"
#include "visname.h"
#include "visposition.h"
#include "vispath.h"

/* Constructor
 * Empty.
 */
Visualiser::Visualiser(QWidget*)  { }

/* Constructor
 * Initalises the visualiser data.
 */
Visualiser::Visualiser(DataModel *dataModelRef) {
    this->dataModelRef = dataModelRef;

    // Default visualiser config
    this->config = VisConfig();
    this->config.elements.push_back(new VisID());
    this->config.elements.push_back(new VisName());
    this->config.elements.push_back(new VisPosition());
    this->config.elements.push_back(new VisPath());

    this->click.x = 0.0;
    this->click.y = 0.0;
}

void Visualiser::refreshVisualisation()
{
    repaint();
}

/* paintEvent
 * Override. Called to re-draw the widget.
 */
void Visualiser::paintEvent(QPaintEvent*) {

    // Display the image
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setBackground(QBrush{QColor{200, 200, 200}});
    painter.setWindow(this->rect());
    painter.fillRect(this->rect(), painter.background());

    QPen pen{QColor{255, 255, 255}};
    pen.setWidth(3);

    painter.setPen(pen);

    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        // Get data
        RobotData* robot = dataModelRef->getRobotByIndex(i);
        bool selected = dataModelRef->selectedRobotID == robot->getID();

        // Render the visualisations
        for (size_t j = 0; j < this->config.elements.size(); j++) {
            VisElement* element = this->config.elements.at(j);
            element->render(this, &painter, robot, selected);
        }
    }

    painter.end();
}

/* resizeEvent
 * Override. Called when the widget is resized. Call checkFrameSize to
 * emit a frameSizeChanged signal with the new size.
 */
void Visualiser::resizeEvent(QResizeEvent*) {
//    checkFrameSize();
}

/* mousePressEvent
 * Captures mouse presses when the mouse is within the visualiser bounds.
 */
void Visualiser::mousePressEvent(QMouseEvent* event) {
    // Calculate x and y values as proportions of the image
    click.x = (1.0 * event->x()) / this->size().width();
    click.y = (1.0 * event->y()) / this->size().height();

    // Loop over the robots looking for any within a threshold of the click
    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        RobotData* robot = dataModelRef->getRobotByIndex(i);

        float dx = std::abs(robot->getPos().position.x - click.x);
        float dy = std::abs(robot->getPos().position.y - click.y);

        if (dx < 0.02 && dy < 0.02) {
            // Signal that a robot has been selected
            emit robotSelectedInVisualiser(robot->getID());

            // Max one selection per click
            return;
        }
    }
}

