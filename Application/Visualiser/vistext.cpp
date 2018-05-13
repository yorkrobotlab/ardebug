/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot id.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "vistext.h"
#include "../Core/settings.h"

#include <QPainter>
#include <QPainterPath>
#include <limits>

/* Constructor
 * Initialise all setttings
 */
VisText::VisText(void) {
    setType(VisType::ID);
    setEnabled(true);
//    setSelectedOnly(false);
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisText::~VisText() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* render
 * Render this visualisation for one robot.
 */
void VisText::render(QWidget* , QPainter* painter, RobotData *robot, bool selected, QRectF rect) {

    if(!selected)
        return;

    double x = rect.x() + (rect.width() * robot->getPos().position.x) + 15;
    double y = rect.y() + (rect.height() * robot->getPos().position.y) - 10;

    auto font = painter->font();
    auto pen  = painter->pen();

    painter->setBackgroundMode(Qt::BGMode::OpaqueMode);
    painter->setBackground(QBrush{QColor{0, 0, 0, 100}});

    painter->setFont(font);
    painter->setPen(pen);
    painter->drawText(QRectF{x, y, std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}, Qt::TextWordWrap, text);
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisText::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new IDSettingsDialog(this);
    return settingsDialog;
}

void VisText::setText(QString newText)
{
    text = newText;
}

QString VisText::getText()
{
    return text;
}

void VisText::addLine(QString newLine)
{
    if(text.length() > 0)
        text += "\n";

    text += newLine;
}

void VisText::resetText()
{
    text = "";
}
