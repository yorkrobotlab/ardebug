/* robotdata.cpp
 *
 * This class encapsulates data related to a single robot.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "robotdata.h"
#include "util.h"

/* Construtor
 * Create a robot instance, with an ID and a name. Set other values to
 * defaults.
 */
RobotData::RobotData(int id, QString name) {
    this->id = id;
    this->name = name;
    this->state = "Unknown";
    this->stateTransitionIndex = 0;
    setPos(0.0f, 0.0f);
    setAngle(0);
    setColour(colourGen());
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
    if (this->state != state) {
        // If this state has not been seen before add it to the list of known states
        if (!this->knownStates.stringList().contains(state)) {
            QStringList list = this->knownStates.stringList();
            list.append(state);
            this->knownStates.setStringList(list);
        }

        // Update the state transition history
        updateStateTransitionHistory(state);

        // Update the current state
        this->state = state;
    }
}

/* updateStateTransitionHistory
 * Add an entry to the transition history from the old state
 * to the new one, at the current time.
 */
void RobotData::updateStateTransitionHistory(QString newState) {
    // Create the transition
    StateTransition trans;
    trans.oldState = this->state;
    trans.newState = newState;
    trans.time = QTime::currentTime();

    // Add it to the circular queue
    this->stateTransitionHistory[this->stateTransitionIndex] = trans;
    this->stateTransitionIndex++;
    if (this->stateTransitionIndex == 10) {
        this->stateTransitionIndex = 0;
    }

    // Update the transition list
    QStringList list;

    // Iterate over list
    for(int i = 0; i < 10; i++) {
        // Start from the most recent transition and work backwards
        int idx = this->stateTransitionIndex - i - 1;

        // Adhere to the circular queue structure
        if (idx < 0) idx += 10;

        // If the transition at idx is not null, add it to the list
        StateTransition trans = this->stateTransitionHistory[idx];
        if (!trans.time.isNull()) {
            list.append(trans.time.toString("HH:mm:ss:zzz") + " " + trans.oldState + " -> " + trans.newState);
        }
    }

    // Update the string list model
    this->stateTransitionList.setStringList(list);
}

/* getState
 * Get the current state.
 */
QString RobotData::getState(void) {
    return this->state;
}

/* getKnownStates
 * Get the list of known states for this robot.
 */
QStringListModel* RobotData::getKnownStates(void) {
    return &this->knownStates;
}

/* getStateTransitionList
 * Get the state transition history in a list form.
 */
QStringListModel* RobotData::getStateTransitionList(void) {
    return &this->stateTransitionList;
}

/* getID
 * Get the robot ID number.
 */
int RobotData::getID(void) {
    return this->id;
}

/* setName
 * Set the robot name string;
 */
void RobotData::setName(QString name) {
    this->name = name;
}

/* getName
 * Get the robots name string.
 */
QString RobotData::getName(void) {
    return this->name;
}

/* getIDConst
 * Copy of getID, declared cost. Used for list sorting.
 */
int RobotData::getIDConst(void) const {
    return this->id;
}

/* getAngle
 * Get the angle the robot is facing.
 */
int RobotData::getAngle(void) {
    return this->angle;
}

/* setAngle
 * Set the robot's angle.
 */
void RobotData::setAngle(int angle) {
    this->angle = angle;
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
