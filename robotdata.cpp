/* robotdata.cpp
 *
 * This class encapsulates data related to a single robot.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "robotdata.h"

/* Construtor
 * Create a robot instance, with an ID and a name. Set other values to
 * defaults.
 */
RobotData::RobotData(int id, QString name) {
    this->id = id;
    this->name = name;
    setPos(0.0f, 0.0f);
    setState("Unknown");
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
    this->pos.x = x;
    this->pos.y = y;
}

/* getPos
 * Get the position coords.
 */
Vector2D RobotData::getPos(void) {
    return this->pos;
}

/* setState
 * Update the current state string.
 */
void RobotData::setState(QString state) {
    this->state = state;
}

/* getState
 * Get the current state.
 */
QString RobotData::getState(void) {
    return this->state;
}

/* getID
 * Get the robot ID number.
 */
int RobotData::getID(void) {
    return this->id;
}

/* getIDConst
 * Copy of getID, declared cost. Used for list sorting.
 */
int RobotData::getIDConst(void) const {
    return this->id;
}

/* getName
 * Get the robots name string.
 */
QString RobotData::getName(void) {
    return this->name;
}
