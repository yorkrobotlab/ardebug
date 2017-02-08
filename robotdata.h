#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>

#include "util.h"

class RobotData
{
    QString id;
    Vector2D pos;
    QString state;

public:
    RobotData(const QString id);
    ~RobotData(void);

    void setPos(float x, float y);
    Vector2D getPos(void);

    void setState(QString state);
    QString getState(void);

    QString getID(void);
};

#endif // ROBOTDATA_H
