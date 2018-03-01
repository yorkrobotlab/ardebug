#ifndef ROBOTDATA_H
#define ROBOTDATA_H

#include <QString>
#include <QStringListModel>
#include <QTableWidget>

#include <opencv2/core/core.hpp>

#include "../Core/util.h"

#define STATE_HISTORY_COUNT     10
#define POS_HISTORY_COUNT       30
#define PROX_SENS_COUNT         8

#define POS_HISTORY_INTERVAL    10

enum ValueType
{
    String,
    Double,
    Bool,
    Object,
    Array,
    Unknown
};

struct RobotStateValue
{
    ValueType type;

    QString stringValue = "";
    double doubleValue = 0;
    bool boolValue = false;
    QList<RobotStateValue> arrayValue = {};
    QMap<QString, RobotStateValue> objectValue;
};

class RobotData
{
    // Identifiers
    QString id;

    // State data
    QMap<QString, RobotStateValue> values;

    // Position
    Pose pos;
    Pose posHistory[POS_HISTORY_COUNT];
    int posHistoryIndex;
    int posHistoryFrameCount;

    // Colour
    cv::Scalar colour;

public:
    RobotData(QString id);
    ~RobotData(void);

    QString getID(void);
    void setID(QString newId);
    QString getIDConst(void) const;

    Pose getPos(void);
    void getPosHistory(Pose* result);
    void setPos(float x, float y);

    int getAngle(void);
    void setAngle(int angle);

    cv::Scalar getColour(void);
    void setColour(cv::Scalar colour);

    void setBoolValue(QString name, bool value)
    {
        auto& val = values[name];
        val.type = Bool;
        val.boolValue = value;
    }

    void setDoubleValue(QString name, double value)
    {
        auto& val = values[name];
        val.type = Double;
        val.doubleValue = value;
    }

    void setStringValue(QString name, QString value)
    {
        auto& val = values[name];
        val.type = String;
        val.stringValue = value;
    }

    bool getBoolValue(QString name) { return values[name].boolValue; }
    double getDoubleValue(QString name) { return values[name].doubleValue; }
    QString getStringValue(QString name) { return values[name].stringValue; }

    QList<RobotStateValue>& getArrayValue(QString name)
    {
        if(!values.contains(name))
        {
            values[name].type = Array;
            values[name].arrayValue = {};
        }

        return values[name].arrayValue;
    }

    QMap<QString, RobotStateValue>& getObjectValue(QString name)
    {
        if(!values.contains(name))
        {
            values[name].type = Object;
            values[name].objectValue = {};
        }

        return values[name].objectValue;
    }
private:
    void updatePositionHistory(void);
};

#endif // ROBOTDATA_H
