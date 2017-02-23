/* visualiser.cpp
 *
 * This class encapsulates the visualiser Qt widget for displaying the camera feed, acquired
 * via opencv.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visualiser.h"
#include "machinevision.h"
#include "settings.h"

#include <stdio.h>
#include <math.h>

#include <QLayout>

#include "visposition.h"
#include "visdirection.h"
#include "visname.h"
#include "visstate.h"
#include "visproximity.h"

using namespace cv;

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
    this->config.elements.push_back(new VisName());
    this->config.elements.push_back(new VisState());
    this->config.elements.push_back(new VisPosition());
    this->config.elements.push_back(new VisDirection());
    this->config.elements.push_back(new VisProximity());
}

/* showImage
 * Display the supplied opencv image.
 */
void Visualiser::showImage(const Mat& image) {
    // Iterate over the list of robots
    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        // Get data
        RobotData* robot = dataModelRef->getRobotByIndex(i);
        bool selected = dataModelRef->selectedRobotID == robot->getID();

        // Render the visualisations
        for (size_t j = 0; j < this->config.elements.size(); j++) {
            VisElement* element = this->config.elements.at(j);
            element->render(image, robot, selected);
        }
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
    double fullWidth = Settings::instance()->getCameraImageSize().x; // 2096
    double fullHeight = Settings::instance()->getCameraImageSize().y; // 1180

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
