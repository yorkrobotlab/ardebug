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
 * Create a timer to poll for camera data.
 */
Visualiser::Visualiser(QWidget *parent) {
    cameraTimer = new QTimer();
    connect(cameraTimer, SIGNAL(timeout()), this, SLOT(getImageFromCamera()));
}

/* startVis
 * Start polling for camera data to display in the visualiser.
 */
void Visualiser::startVis() {
    if(!cameraTimer->isActive()) {
        cameraTimer->start(41);
    }
}

/* stopVis
 * Stop polling for camera data.
 */
void Visualiser::stopVis() {
    cameraTimer->stop();
}

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

    // Widget size must be fixed to image size
    this->setFixedSize(image.cols, image.rows);

    // Redraw
    repaint();
}

/* paintEvent
 * Override. Called to re-draw the widget.
 */
void Visualiser::paintEvent(QPaintEvent*) {
    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
    painter.end();
}

/* getImageFromCamera
 * Slot. Called by the camera poll timer. Calls a function in the machine vision file to get
 * the latest frame of video data. Pause the timer whilst doing it.
 */
void Visualiser::getImageFromCamera() {
    int w = this->size().width();
    int h = this->size().height();
    int size = w < h ? w : h;
    std::cout << "Size: " << w << " " << h << std::endl;

    cameraTimer->stop();
    this->showImage(getFrame(size));
    cameraTimer->start(41);
}
