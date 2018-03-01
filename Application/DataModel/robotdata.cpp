/* robotdata.cpp
 *
 * This class encapsulates data related to a single robot.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "robotdata.h"
#include "../Core/util.h"
#include "../Core/settings.h"
#include <iostream>

#include <QTableWidgetItem>

/* Construtor
 * Create a robot instance, with an ID and a name. Set other values to
 * defaults.
 */
RobotData::RobotData(QString id) {
    // Initialise identifiers
    this->id = id;

    // Initialise odometry
    setPos(0.0f, 0.0f);
    setAngle(0);
    this->posHistoryIndex = 0;
    this->posHistoryFrameCount = 0;
    bzero(this->posHistory, sizeof(Vector2D) * POS_HISTORY_COUNT);

    // Generate colour
    setColour(cv::Scalar(255, 255, 255));
}

/* Destructor
 * Release allocated memory.
 */
RobotData::~RobotData(void) {
    // Destruct
}

/* setPos
 * Update the position with new coords.
 */
void RobotData::setPos(float x, float y) {
    // Then update the current position
    this->pos.position.x = x;
    this->pos.position.y = y;

    // First update the history
    updatePositionHistory();
}

/* getPos
 * Get the position coords.
 */
Pose RobotData::getPos(void) {
    return this->pos;
}

/* getPosHistory
 * Gets the historical position data in order of ascending age
 */
void RobotData::getPosHistory(Pose* result) {
    int i = 0;
    int idx = posHistoryIndex - 1;

    if (idx < 0) {
        idx = POS_HISTORY_COUNT - 1;
    }

    while (idx != posHistoryIndex) {
        result[i] = posHistory[idx];

        i++;
        idx--;

        if (idx < 0) {
            idx = POS_HISTORY_COUNT - 1;
        }
    }

    result[i] = posHistory[posHistoryIndex];
}

/* updatePositionHistory
 * Check if enough frames have elapsed to insert the current position
 * into the position history array.
 */
void RobotData::updatePositionHistory(void) {
    if (posHistoryFrameCount == 0) {
        posHistory[posHistoryIndex].position.x = pos.position.x;
        posHistory[posHistoryIndex].position.y = pos.position.y;

        posHistoryIndex++;

        if (posHistoryIndex >= POS_HISTORY_COUNT) {
            posHistoryIndex = 0;
        }
    }

    posHistoryFrameCount++;

    if (posHistoryFrameCount >= Settings::instance()->getPosHistorySampleInterval()) {
        posHistoryFrameCount = 0;
    }
}

/* getID
 * Get the robot ID number.
 */
QString RobotData::getID(void) {
    return this->id;
}

/* setName
 * Set the robot name string;
 */
void RobotData::setID(QString newId) {
    this->id = newId;
}

/* getIDConst
 * Copy of getID, declared cost. Used for list sorting.
 */
QString RobotData::getIDConst(void) const {
    return this->id;
}

/* getAngle
 * Get the angle the robot is facing.
 */
int RobotData::getAngle(void) {
    return this->pos.orientation;
}

/* setAngle
 * Set the robot's angle.
 */
void RobotData::setAngle(int angle) {
    this->pos.orientation = angle;
}

/* getColour
 * Get the colour used for this robot in the UI
 */
cv::Scalar RobotData::getColour(void) {
    return this->colour;
}

/* setColour
 * Set the colour used for this robot in the UI
 */
void RobotData::setColour(cv::Scalar colour) {
    this->colour = colour;
}
