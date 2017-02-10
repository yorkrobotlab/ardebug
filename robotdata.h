#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>
#include <QStringListModel>

#include <opencv2/core.hpp>

#include "util.h"

class RobotData
{
    int id;
    QString name;
    QString state;
    QStringListModel knownStates;

    StateTransition stateTransitionHistory[10];
    int stateTransitionIndex;
    QStringListModel stateTransitionList;

    Vector2D pos;
    int angle;

    cv::Scalar colour;

public:
    RobotData(int id, QString name);
    ~RobotData(void);

    int getID(void);
    int getIDConst(void) const;

    QString getName(void);
    void setName(QString name);

    QString getState(void);
    QStringListModel* getKnownStates(void);
    QStringListModel* getStateTransitionList(void);
    void setState(QString state);

    Vector2D getPos(void);
    void setPos(float x, float y);

    int getAngle(void);
    void setAngle(int angle);

    cv::Scalar getColour(void);
    void setColour(cv::Scalar colour);

private:
    void updateStateTransitionHistory(QString newState);
};

#endif // ROBOTDATA_H
