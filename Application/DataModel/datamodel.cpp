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

/* newData
 * Slot. Called when new data arrives.
 */
void DataModel::newData(const QString &dataString) {

    if(dataString[0]=='{')
        Log::instance()->logMessage("Got message: " + dataString, true);

    // Parse the received data as a JSON string
    QJsonDocument j = QJsonDocument::fromJson(dataString.toUtf8());
    QJsonObject message = j.object();

    // Check that
    if(!message.contains("id"))
        return;

    QString robotId = message["id"].toString();
    RobotData* robot = getRobotByID(robotId);

    Log::instance()->logMessage("Message from robot " + robotId, true);

    // Signal to the UI that new data is available
    emit modelChanged(true);
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

/* parseProximityPacket
 * Parses raw proximity sensor data values into the data model
 */
void DataModel::parseProximityPacket(RobotData *robot, QStringList data, bool background) {
    int proxData[PROX_SENS_COUNT] = {0};
    int mask = 0;

    // data 0 and 1 are ID and Type. Iterate from data 2 onwards
    for (int i = 2; i < data.length() && i < PROX_SENS_COUNT + 2; i++) {
        // Read string into int
        bool ok = false;
        int prox = data[i].toInt(&ok);

        // If valid value, store in priximity data array
        if (ok && prox >= 0) {
            proxData[i - 2] = prox;

            // Set mask bit to show value at i is valid
            mask |= 1 << (i-2);
        }
    }

    // Update the robots
    if (background) {
        robot->updateBackgroundIR(proxData, mask);
    } else {
        robot->updateProximitySensorData(proxData, mask);
    }

    // Log the data
    data.removeFirst();
    data.removeFirst();
    if (background) {
        Log::instance()->logMessage("Robot " + robot->getID() + " - Background IR Data: " + data.join(" "), false);
    } else {
        Log::instance()->logMessage("Robot " + robot->getID() + " - IR Data: " + data.join(" "), false);
    }
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
