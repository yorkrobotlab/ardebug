/* datamodel.cpp
 *
 * This class encapsulates the data model used to store data on the active
 * robots.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "datamodel.h"
#include "../Core/util.h"
#include "../Core/log.h"
#include "../Core/settings.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <iostream>
#include <stdio.h>

using namespace  std;

/* Constructor
 * Set up the list of robot data (vector).
 */
DataModel::DataModel(QObject *parent) : QObject(parent)
{
    // Instantiate the data model here
    robotDataList.reserve(10);

    // Create a  model for the list of active robot ids
    robotListModel = new QStringListModel();

    // Initially no robot selected
    selectedRobotID = -1;

    // Initialise the average position
    averageRobotPos.x = 0.0f;
    averageRobotPos.y = 0.0f;
}

/* Destructor
 * Release allocated memory.
 */
DataModel::~DataModel(void) {
    delete robotListModel;

    for (size_t i = 0; i < robotDataList.size(); i++) {
        delete robotDataList[i];
    }
    robotDataList.clear();
}

/* getRobotByID
 * Return a pointer to the data of the robot with the given ID. Returns null
 * if ID cannot be found.
 */
RobotData* DataModel::getRobotByID(QString id) {
    auto matchingRobot = std::find_if(robotDataList.begin(), robotDataList.end(), [&id](RobotData* r){return r->getID() == id;});
    if(matchingRobot != robotDataList.end())
        return *matchingRobot;

    return nullptr;
}

/* getRobotList
 * Returns a QStringListModel of all the robots being tracked.
 */
QStringListModel* DataModel::getRobotList(void) {
    QStringList list;

    // Loop over all the robots
    for(size_t i = 0; i < robotDataList.size(); i++) {
        RobotData* d = (RobotData*)robotDataList.at(i);

        // Append the robots ID and name
        QString str(d->getID());
        list.append(str);
    }

    // Return the list model
    robotListModel->setStringList(list);
    return robotListModel;
}

RobotData* DataModel::setSelectedRobot(int idx)
{
    selectedRobotID = robotDataList[idx]->getID();
    return robotDataList[idx];
}

/* getRobotCount
 * Returns the number of robots for which data is currently
 * stored;
 */
int DataModel::getRobotCount(void) {
    return robotDataList.size();
}

ValueType typeOfJsonValue(QJsonValue val)
{
    if(val.isString())
        return String;

    if(val.isArray())
        return Array;

    if(val.isBool())
        return Bool;

    if(val.isDouble())
        return Double;

    if(val.isObject())
        return Object;

    return Unknown;
}

void parseBoolValue(QMap<QString, RobotStateValue>& dict, QString name, QJsonValueRef val);
void parseDoubleValue(QMap<QString, RobotStateValue>& dict, QString name, QJsonValueRef val);
void parseStringValue(QMap<QString, RobotStateValue>& dict, QString name, QJsonValueRef val);
void populateListFromJson(QList<RobotStateValue>& array, QJsonArray vals);
void populateObjectFromJson(QMap<QString,RobotStateValue>& obj, QJsonObject jsonObj);


void parseBoolValue(QMap<QString, RobotStateValue>& dict, QString name, QJsonValueRef val)
{
    RobotStateValue v;
    v.type = Bool;
    v.boolValue = val.toBool();
    dict[name] = v;
}

void parseDoubleValue(QMap<QString, RobotStateValue>& dict, QString name, QJsonValueRef val)
{
    RobotStateValue v;
    v.type = Double;
    v.doubleValue = val.toDouble();
    dict[name] = v;
}

void parseStringValue(QMap<QString, RobotStateValue>& dict, QString name, QJsonValueRef val)
{
    RobotStateValue v;
    v.type = String;
    v.stringValue = val.toString();
    dict[name] = v;
}

void populateListFromJson(QList<RobotStateValue>& array, QJsonArray vals)
{
    auto type = typeOfJsonValue(vals[0]);
    switch(type)
    {
    case Double:
    {
        for(auto v_a : vals)
        {
            RobotStateValue v_i;
            v_i.type = Double;
            v_i.doubleValue = v_a.toDouble();
            array.push_back(v_i);
        }
        break;
    }
    case Bool:
    {
        for(auto v_a : vals)
        {
            RobotStateValue v_i;
            v_i.type = Bool;
            v_i.boolValue = v_a.toBool();
            array.push_back(v_i);
        }
        break;
    }
    case String:
    {
        for(auto v_a : vals)
        {
            RobotStateValue v_i;
            v_i.type = String;
            v_i.stringValue = v_a.toString();
            array.push_back(v_i);
        }
        break;
    }
    case Array:
    {
        for(auto v_a : vals)
        {
            RobotStateValue v_i;
            v_i.type = Array;
            v_i.arrayValue = {};
            populateListFromJson(v_i.arrayValue, v_a.toArray());
        }
        break;
    }
    case Object:
    {
        for(auto v_a : vals)
        {
            RobotStateValue v_i;
            v_i.type = Object;
            v_i.objectValue = {};
            populateObjectFromJson(v_i.objectValue, v_a.toObject());
        }
        break;
    }
    default:
    {

    }
    }
}

void populateObjectFromJson(QMap<QString,RobotStateValue>& obj, QJsonObject jsonObj)
{
    for(auto key : jsonObj.keys())
    {
        auto type = typeOfJsonValue(jsonObj[key]);
        switch(type)
        {
        case Double:
        {
            RobotStateValue v_i;
            v_i.type = Double;
            v_i.doubleValue = jsonObj[key].toDouble();
            obj[key] = v_i;
            break;
        }
        case String:
        {
            RobotStateValue v_i;
            v_i.type = String;
            v_i.stringValue = jsonObj[key].toString();
            obj[key] = v_i;
            break;
        }
        case Bool:
        {
            RobotStateValue v_i;
            v_i.type = Bool;
            v_i.boolValue = jsonObj[key].toBool();
            obj[key] = v_i;
            break;
        }
        case Array:
        {
            RobotStateValue v_i;
            v_i.type = Array;
            v_i.arrayValue = {};
            populateListFromJson(v_i.arrayValue, jsonObj[key].toArray());
            obj[key] = v_i;
            break;
        }
        case Object:
        {
            RobotStateValue v_i;
            v_i.type = Object;
            v_i.objectValue = {};
            populateObjectFromJson(v_i.objectValue, jsonObj[key].toObject());
            obj[key] = v_i;
            break;
        }
        default:
        {

        }
        }
    }
}

/* newData
 * Slot. Called when new data arrives.
 */
void DataModel::newData(const QString &dataString) {
    // Parse the received data as a JSON string
    QJsonDocument j = QJsonDocument::fromJson(dataString.toUtf8());
    QJsonObject message = j.object();

    // Check that
    if(!message.contains("id"))
        return;

    QString robotId = message["id"].toString();
    message.remove("id");
    addRobotIfNotExist(robotId);
    RobotData* robot = getRobotByID(robotId);

    Log::instance()->logMessage("Message from robot " + robotId, true);

    if(message.contains("pose"))
    {
        QJsonObject jsonPose = message["pose"].toObject();
        Pose p;
        p.orientation = jsonPose["orientation"].toDouble();
        p.position.x = jsonPose["x"].toDouble();
        p.position.y = jsonPose["y"].toDouble();
        robot->setPos(p.position.x, p.position.y);
        robot->setAngle(p.orientation);

        message.remove("pose");
    }

    for(QString key : message.keys())
    {
        auto val = message[key];
        switch(typeOfJsonValue(val))
        {
        case Bool:
        {
            robot->setBoolValue(key, val.toBool());
            break;
        }
        case Double:
        {
            robot->setDoubleValue(key, val.toDouble());
            break;
        }
        case String:
        {
            robot->setStringValue(key, val.toString());
            break;
        }
        case Object:
        {
            auto& v = robot->getObjectValue(key);
            populateObjectFromJson(v, val.toObject());
            break;
        }
        case Array:
        {
            auto& v = robot->getArrayValue(key);
            populateListFromJson(v, val.toArray());
            break;
        }
        default:
        {

        }
        }
    }

    // Signal to the UI that new data is available
    emit modelChanged(true);
}

void DataModel::newRobotPosition(QString id, Pose p)
{
    addRobotIfNotExist(id);
    RobotData* robot = getRobotByID(id);
    robot->setPos(p.position.x, p.position.y);
    robot->setAngle(p.orientation);
}

void DataModel::addRobotIfNotExist(QString id)
{
    RobotData* r = getRobotByID(id);
    if(r == nullptr)
        robotDataList.push_back(new RobotData{id});
}

/* parsePositionPacket
 * Parses floating point data from the string elements of a position packet.
 */
void DataModel::parsePositionPacket(RobotData* robot, QString xString, QString yString, QString aString) {
    bool ok;
    float x = xString.toFloat(&ok);

    if(!ok) {
        return;
    }

    float y = yString.toFloat(&ok);

    if(!ok) {
        return;
    }

    int a = aString.toInt(&ok);

    if(!ok) {
        return;
    }

    robot->setPos(x, y);
    robot->setAngle(a);

    updateAveragePosition();
}

/* deleteRobot
 * Remove a robot from the data model, including all of its data.
 */
void DataModel::deleteRobot(QString id) {
    // Reset the robot selection if it matches to avoid null pointer errors
    if (this->selectedRobotID == id) {
        this->selectedRobotID = "";
    }

    // Retrieve the index of the robot to be deleted. Do not create it not found.
    std::remove_if(robotDataList.begin(), robotDataList.end(), [&id](RobotData* r){ return r->getID() == id; });
}

/* updateAveragePosition
 * Updates the average position.
 */
void DataModel::updateAveragePosition(void) {
    float x = 0.0f;
    float y = 0.0f;
    int robotCount = (int)robotDataList.size();

    for (int idx = 0; idx < robotCount; idx++) {
        RobotData* d = (RobotData*)robotDataList.at(idx);

        x += d->getPos().position.x;
        y += d->getPos().position.y;
    }

    averageRobotPos.x = x/robotCount;
    averageRobotPos.y = y/robotCount;
}
