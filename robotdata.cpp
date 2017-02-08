#include "robotdata.h"

RobotData::RobotData(QString id) {
    this->id = id;
    setPos(0.0f, 0.0f);
    setState("Unknown");
}

RobotData::~RobotData(void) {
    // Destruct
}

void RobotData::setPos(float x, float y) {
    this->pos.x = x;
    this->pos.y = y;
}

Vector2D RobotData::getPos(void) {
    return this->pos;
}

void RobotData::setState(QString state) {
    this->state = state;
}

QString RobotData::getState(void) {
    return this->state;
}

QString RobotData::getID(void) {
    return id;
}
