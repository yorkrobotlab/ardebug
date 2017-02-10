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
    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        // Get data
        RobotData* robot = dataModelRef->getRobotByIndex(i);
        int x = image.cols * robot->getPos().x;
        int y = image.rows * robot->getPos().y;
        int a = robot->getAngle();

        // Draw cross
        line(image, Point(x - 2, y), Point(x + 2, y), Scalar(0, 50, 255));
        line(image, Point(x, y - 2), Point(x, y + 2), Scalar(0, 50, 255));

        // Draw direction
        Point end = Point(x + (int)(10 * cos(a * PI/180)), y + (int)(10 * sin(a * PI/180)));
        line(image, Point(x, y), end, Scalar(0, 0, 255));
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
    // Determine the limiting size
    int w = this->size().width();
    int h = this->size().height();
    int size = w < h ? w : h;

    // Do not allow <0 sizes
    if (size <= 0) {
        size = 1;
    }

    // Emit the new size as a signal
    emit frameSizeChanged(size);
}
