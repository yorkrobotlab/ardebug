#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>
#include <QStringListModel>
#include <QTableWidget>

#include <opencv2/core.hpp>

#include "util.h"

#define STATE_HISTORY_COUNT     10
#define POS_HISTORY_COUNT       30
#define PROX_SENS_COUNT         8

#define POS_HISTORY_INTERVAL    10

class RobotData
{
    // Identifiers
    int id;
    QString name;

    // State data
    QString state;
    QStringListModel knownStates;
    QStringListModel stateTransitionList;
    StateTransition stateTransitionHistory[STATE_HISTORY_COUNT];
    int stateTransitionIndex;

    // Position
    Vector2D pos;
    Vector2D posHistory[POS_HISTORY_COUNT];
    int posHistoryIndex;
    int posHistoryFrameCount;

    // Direction
    int angle;

    // Colour
    cv::Scalar colour;

    // Sensor Data
    int proximityData[PROX_SENS_COUNT];
    int backgroundIR[PROX_SENS_COUNT];

    // Custom Data
    std::map<QString, QString> customData;

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
    void getPosHistory(Vector2D* result);
    void setPos(float x, float y);

    int getAngle(void);
    void setAngle(int angle);

    cv::Scalar getColour(void);
    void setColour(cv::Scalar colour);

    void updateProximitySensorData(int* data, int mask);
    int getProximitySensorData(int sensor);

    void updateBackgroundIR(int* data, int mask);
    int getBackgroundIR(int sensor);

    void insertCustomData(QString key, QString value);
    void populateCustomDataTable(QTableWidget* table);
    QString getCustomData(QString key);

private:
    void updateStateTransitionHistory(QString newState);
    void updatePositionHistory(void);
};

#endif // ROBOTDATA_H
