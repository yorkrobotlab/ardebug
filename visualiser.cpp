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

#include "visid.h"
#include "visname.h"
#include "visstate.h"
#include "visposition.h"
#include "visdirection.h"
#include "visproximity.h"
#include "vispath.h"
#include "viscustom.h"

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
    this->config.elements.push_back(new VisID());
    this->config.elements.push_back(new VisName());
    this->config.elements.push_back(new VisState());
    this->config.elements.push_back(new VisPosition());
    this->config.elements.push_back(new VisDirection());
    this->config.elements.push_back(new VisProximity());
    this->config.elements.push_back(new VisPath());
    this->config.elements.push_back(new VisCustom());

    this->click.x = 0.0;
    this->click.y = 0.0;
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

    // Draw click point
    //int x = image.cols * click.x;
    //int y = image.rows * click.y;

    //line(image, cv::Point(x - 4, y), cv::Point(x + 4, y), cv::Scalar(255, 0, 0), 1);
    //line(image, cv::Point(x, y - 4), cv::Point(x, y + 4), cv::Scalar(255, 0, 0), 1);

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
    int x = (this->size().width() - _qimage.width()) / 2;
    int y = (this->size().height() - _qimage.height()) / 2;

    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(x,y), _qimage);
    painter.end();
}

/* resizeEvent
 * Override. Called when the widget is resized. Call checkFrameSize to
 * emit a frameSizeChanged signal with the new size.
 */
void Visualiser::resizeEvent(QResizeEvent*) {
    checkFrameSize();
}

/* mousePressEvent
 * Captures mouse presses when the mouse is within the visualiser bounds.
 */
void Visualiser::mousePressEvent(QMouseEvent* event) {
    // Calculate margins
    int xMargin = (this->size().width() - _qimage.width())/2;
    int yMargin = (this->size().height() - _qimage.height())/2;

    // Calculate x and y values as proportions of the image
    click.x = (float)(event->x() - xMargin) / (float)(this->size().width() - (2 * xMargin));
    click.y = (float)(event->y() - yMargin) / (float)(this->size().height() - (2 * yMargin));

    // Loop over the robots looking for any within a threshold of the click
    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        RobotData* robot = dataModelRef->getRobotByIndex(i);

        float dx = std::abs(robot->getPos().x - click.x);
        float dy = std::abs(robot->getPos().y - click.y);

        if (dx < 0.02 && dy < 0.02) {
            // Signal that a robot has been selected
            emit robotSelectedInVisualiser(robot->getID());

            // Max one selection per click
            return;
        }
    }
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
