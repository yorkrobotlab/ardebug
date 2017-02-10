#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>

#include "util.h"

class RobotData
{
    int id;
    QString name;
    QString state;

    Vector2D pos;
    int angle;

public:
    RobotData(int id, QString name);
    ~RobotData(void);

    int getID(void);
    int getIDConst(void) const;

    QString getName(void);
    void setName(QString name);

    QString getState(void);
    void setState(QString state);

    Vector2D getPos(void);
    void setPos(float x, float y);

    int getAngle(void);
    void setAngle(int angle);
};

#endif // ROBOTDATA_H
