#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>
#include <QStringListModel>

#include <opencv2/core.hpp>

#include "util.h"

#define PROX_SENS_COUNT         10

class RobotData
{
    // Identifiers
    int id;
    QString name;

    // State data
    QString state;
    QStringListModel knownStates;
    QStringListModel stateTransitionList;
    StateTransition stateTransitionHistory[PROX_SENS_COUNT];
    int stateTransitionIndex;

    // Odometry
    Vector2D pos;
    int angle;

    // Colour
    cv::Scalar colour;

    // Sensor Data
    int proximityData[10];

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

    void updateProximitySensorData(int* data, int mask);
    int getProximitySensorData(int sensor);

private:
    void updateStateTransitionHistory(QString newState);
};

#endif // ROBOTDATA_H
