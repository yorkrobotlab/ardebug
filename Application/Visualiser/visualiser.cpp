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

#include "vistext.h"
#include "visposition.h"

#include <iostream>

/* Constructor
 * Empty.
 */
//Visualiser::Visualiser(QWidget*)  { }

/* Constructor
 * Initalises the visualiser data.
 */
Visualiser::Visualiser(DataModel *dataModelRef, ARCameraThread* cameraThread) {
    this->dataModelRef = dataModelRef;

    // Default visualiser config
    this->config = VisConfig();
    textVis = new VisText;
    this->config.elements.push_back(textVis);
    this->config.elements.push_back(new VisPosition);

    this->click.x = 0.0;
    this->click.y = 0.0;

    backgroundImage.fill(QColor{200, 200, 200});

    this->cameraThread = cameraThread;
    connect(cameraThread, SIGNAL(newVideoFrame(cv::Mat&)), this, SLOT(newVideoFrame(cv::Mat&)));
}

void Visualiser::refreshVisualisation()
{
//    repaint();

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

    double width = this->width();
    double height = this->height();
    double xOffset = 0;
    double yOffset = 0;

    xOffset = 0.5*(this->width() - backgroundImage.width());
    yOffset = 0.5*(this->height() - backgroundImage.height());
    width = backgroundImage.width();
    height = backgroundImage.height();

    painter.drawImage(xOffset, yOffset, backgroundImage);

    QPen pen{QColor{255, 255, 255}};
    pen.setWidth(3);

    painter.setPen(pen);

    const auto& selectedId = dataModelRef->selectedRobotID;

    std::vector<RobotData*> selectedRobots;
    std::vector<RobotData*> unselectedRobots;

    for (int i = 0; i < dataModelRef->getRobotCount(); i++) {
        // Get data
        RobotData* robot = dataModelRef->getRobotByIndex(i);

        if(robot->getID() == selectedId)
            selectedRobots.push_back(robot);
        else
            unselectedRobots.push_back(robot);
    }

    for(auto robot : unselectedRobots)
        renderSingleRobot(robot, false, painter, xOffset, yOffset, width, height);

    for(auto robot : selectedRobots)
        renderSingleRobot(robot, true, painter, xOffset, yOffset, width, height);

    painter.end();
}

void Visualiser::renderSingleRobot(RobotData* robot, bool selected, QPainter& painter, double xOffset, double yOffset, double width, double height){
    // @EXTEND: Add other data types
    textVis->resetText();
    textVis->addLine("ID:   " + robot->getID());
    for(auto& key : robot->getKeys())
    {
        if(!robot->valueShouldBeDisplayed(key))
            continue;

        std::stringstream ss;
        ss<<key.toStdString();
        ss<<": ";

        auto type = robot->getValueType(key);

        if(type == String) ss<<robot->getStringValue(key).toStdString();

        if(type == Double) ss<<robot->getDoubleValue(key);

        if(type == Bool) ss<<(robot->getBoolValue(key) ? "True" : "False");

        if(type == Array)
        {
            auto arr = robot->getArrayValue(key);
            ss<<"[ ";
            for(int i = 0; i < arr.size(); ++i)
            {
                if(i > 0) ss<<"   ";
                auto item = arr[i];
                if(item.type == String) ss<<'"'<<item.stringValue.toStdString()<<'"';
                else if(item.type == Double) ss<<item.doubleValue;
                else if(item.type == Bool) ss<<(item.boolValue ? "True" : "False");
                else ss<<"Unsupported";
            }
            ss<<" ]";
        }

        if(type == Object)
        {
            auto obj = robot->getObjectValue(key);
            ss<<"{ ";
            for(auto& key : obj.keys())
            {
                ss<<key.toStdString()<<": ";
                auto& item = obj[key];
                if(item.type == String) ss<<'"'<<item.stringValue.toStdString()<<'"';
                else if(item.type == Double) ss<<item.doubleValue;
                else if(item.type == Bool) ss<<(item.boolValue ? "True" : "False");
                else ss<<"Unsupported";
                ss<<"   ";
            }
            ss<<" }";
        }

        textVis->addLine(QString::fromStdString(ss.str()));
    }

    // Render the visualisations
    for (size_t j = 0; j < this->config.elements.size(); j++) {
        VisElement* element = this->config.elements.at(j);
        element->render(this, &painter, robot, selected, QRectF{xOffset, yOffset, width, height});
    }
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
    double xOffset = 0.5*(this->width() - backgroundImage.width());
    double yOffset = 0.5*(this->height() - backgroundImage.height());

    // Calculate x and y values as proportions of the image
    click.x = (1.0 * event->x() - xOffset) / backgroundImage.width();
    click.y = (1.0 * event->y() - yOffset) / backgroundImage.height();

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

void Visualiser::newVideoFrame(cv::Mat& newImage)
{
    static cv::Mat image;
    cv::cvtColor(newImage, image, cv::COLOR_BGR2RGB);

    double xScale = (1.0 * this->width())/image.cols;
    double yScale = (1.0 * this->height())/image.rows;
    double scaleFactor;

    int newX, newY;

    if(xScale < yScale)
    {
        newX = image.cols * xScale;
        newY = image.rows * xScale;
        scaleFactor = xScale;
    }
    else
    {
        newX = image.cols * yScale;
        newY = image.rows * yScale;
        scaleFactor = yScale;
    }

    cv::resize(image, image, cv::Size{newX, newY}, cv::INTER_LINEAR);

    backgroundImage = QImage(image.data, image.cols, image.rows, image.cols*3, QImage::Format_RGB888);

    repaint();

    cameraThread->addPreEmitCall([&](){
        connect(cameraThread, SIGNAL(newVideoFrame(cv::Mat&)), this, SLOT(newVideoFrame(cv::Mat&)));
    });
}

