#include "irdataview.h"

#include <QPainter>

IRDataView::IRDataView(QWidget *parent) : QWidget(parent) { }

IRDataView::IRDataView(DataModel *dataModelRef) {
    this->dataModelRef = dataModelRef;
}

/* paintEvent
 * Override. Called to re-draw the widget.
 */
void IRDataView::paintEvent(QPaintEvent*) {
    int maxHeight = this->size().height() - 60;
    int baseLine = this->size().height() - 40;

    // Display the image
    QPainter painter(this);
    painter.fillRect( 0, 0, this->size().width(), this->size().height(), Qt::white);

    if (dataModelRef->selectedRobotID != -1) {
        RobotData* robot = dataModelRef->getRobotByID(dataModelRef->selectedRobotID);

        painter.drawText(10, 10, 300, 20, 0, robot->getName());

        for (int i = 0; i < PROX_SENS_COUNT; i++) {
            int raw = robot->getProximitySensorData(i);
            int barHeight = (int)(maxHeight * ((float)raw/4095.0f));

            painter.fillRect(60 + i * 50, baseLine, 20, -barHeight, Qt::red);
            painter.drawText(60 + i * 50, baseLine + 20, 50, 20, 0, QString::number(robot->getProximitySensorData(i)));
        }
    }

    painter.end();
}
