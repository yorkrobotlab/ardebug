/* visualiser.cpp
 *
 * This class encapsulates the visualiser Qt widget for displaying the camera feed, acquired
 * via opencv.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visualiser.h"
#include "machinevision.h"

#include <stdio.h>
#include <math.h>

#define PI 3.14159265

#include <QLayout>

using namespace cv;

/* Constructor
 * Empty.
 */
Visualiser::Visualiser(QWidget*)  { }

Visualiser::Visualiser(DataModel *dataModelRef) {
    this->dataModelRef = dataModelRef;
}

/* showImage
 * Display the supplied opencv image.
 */
void Visualiser::showImage(const Mat& image) {
    // Iterate over the list of robots
    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        // Get data
        RobotData* robot = dataModelRef->getRobotByIndex(i);
        int x = image.cols * robot->getPos().x;
        int y = image.rows * robot->getPos().y;
        int a = robot->getAngle();
        bool selected = dataModelRef->selectedRobotID == robot->getID();

        // If selected, draw text
        if (selected) {
            putText(image, robot->getName().toStdString(), Point(x + 8, y), FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
            putText(image, robot->getState().toStdString(), Point(x + 8, y + 12), FONT_HERSHEY_SIMPLEX, 0.3, robot->getColour());
        }

        // Draw cross
        //line(image, Point(x - 8, y), Point(x + 8, y), robot->getColour(), selected ? 2 : 1);
        //line(image, Point(x, y - 8), Point(x, y + 8), robot->getColour(), selected ? 2 : 1);

        circle(image, Point(x, y), 8, robot->getColour(), selected ? 2 : 1);

        // Draw direction
        Point end = Point(x + (int)(12 * cos(a * PI/180)), y + (int)(12 * sin(a * PI/180)));
        line(image, Point(x, y), end, robot->getColour(), selected ? 2 : 1);
    }

    // Convert to RGB
    switch (image.type()) {
        case CV_8UC1:
            cvtColor(image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(image, _tmp, CV_BGR2RGB);
            break;
        default:
            break;
    }

    // QImage requires data to be stored continuously
    assert(_tmp.isContinuous());

    // Assign OpenCV's image buffer to the QImage.
    // Each pixel is three bytes, hence bytesPerLine is width * 3
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols * 3, QImage::Format_RGB888);

    // Redraw
    repaint();
}

/* paintEvent
 * Override. Called to re-draw the widget.
 */
void Visualiser::paintEvent(QPaintEvent*) {
    // Calculate the x position
    int x = (this->size().width() / 2) - (_qimage.width() / 2);

    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(x,0), _qimage);
    painter.end();
}

/* resizeEvent
 * Override. Called when the widget is resized. Call checkFrameSize to
 * emit a frameSizeChanged signal with the new size.
 */
void Visualiser::resizeEvent(QResizeEvent*) {
    checkFrameSize();
}

/* checkFrameSize
 * Called to check the frame size and emit it.
 */
void Visualiser::checkFrameSize() {
    // The camera output dimensions are 2096 x 1180
    double fullWidth = 2096.0;
    double fullHeight = 1180.0;

    // Determine the limiting size
    int width = this->size().width();
    int desiredHeight = (int)ceil((width/fullWidth) * fullHeight);

    int height = this->size().height();
    int desiredWidth = (int)ceil((height/fullHeight) * fullWidth);

    // Do not allow sizes of zero or less
    if (width <= 0 || height <= 0) {
        emit frameSizeChanged(1, 1);
        return;
    }

    if (desiredHeight > height) {
        // Height is the limiting dimension
        emit frameSizeChanged(desiredWidth, height);
    } else if (desiredWidth > width) {
        // Width is the limiting dimension
        emit frameSizeChanged(width, desiredHeight);
    } else {
        // Perfect dimensions
        emit frameSizeChanged(desiredWidth, desiredHeight);
    }
}
