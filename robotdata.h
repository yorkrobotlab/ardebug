#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>

#include "util.h"

class RobotData
{
    int id;
    QString name;
    Vector2D pos;
    QString state;

public:
    RobotData(int id, QString name);
    ~RobotData(void);

    void setPos(float x, float y);
    Vector2D getPos(void);

    void setState(QString state);
    QString getState(void);

    void setName(QString name);
    QString getName(void);

    int getID(void);
    int getIDConst(void) const;
};

#endif // ROBOTDATA_H
