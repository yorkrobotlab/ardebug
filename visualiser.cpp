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

#include <QLayout>

/* Constructor
 * Empty.
 */
Visualiser::Visualiser(QWidget*)  { }

/* showImage
 * Display the supplied opencv image.
 */
void Visualiser::showImage(const cv::Mat& image) {
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
