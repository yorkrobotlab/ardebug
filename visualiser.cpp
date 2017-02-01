#include "visualiser.h"

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

void Visualiser::paintEvent(QPaintEvent*) {
    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
    painter.end();
}
