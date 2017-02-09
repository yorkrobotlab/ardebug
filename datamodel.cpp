/* datamodel.cpp
 *
 * This class encapsulates the data model used to store data on the active
 * robots.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "datamodel.h"

#include <iostream>
#include <stdio.h>

using namespace  std;

/* sortRobotDataByID
 * Helper function used when sorting the robots.
 */
bool sortRobotDataByID(RobotData* lhs, RobotData* rhs) {
    return lhs->getID() < rhs->getID();
}

/* Constructor
 * Set up the list of robot data (vector).
 */
DataModel::DataModel(QObject *parent) : QObject(parent)
{
    // Instantiate the data model here
    robotDataList.reserve(10);

    // Create a  model for the list of active robot ids
    robotListModel = new QStringListModel();
}

/* Destructor
 * Release allocated memory.
 */
DataModel::~DataModel(void) {
    delete robotListModel;
}

/* getRobotByIndex
 * Return a pointer to the data of the robot at the given index. Returns null
 * if index is invalid.
 */
RobotData* DataModel::getRobotByIndex(int idx) {
    return robotDataList.at(idx);
}

/* getRobotByID
 * Return a pointer to the data of the robot with the given ID. Returns null
 * if ID cannot be found.
 */
RobotData* DataModel::getRobotByID(int id) {
    for (size_t i = 0; i < robotDataList.size(); i++) {
        RobotData* robot = (RobotData*)robotDataList.at(i);

        if (robot->getID() == id) {
            return robot;
        }
    }

    return 0;
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
        QString str(QString::number(d->getID()) + ": " + d->getName());
        list.append(str);
    }

    // Return the list model
    robotListModel->setStringList(list);
    return robotListModel;
}

/* newData
 * Slot. Called when new data arrives.
 */
void DataModel::newData(const QString &dataString) {
    int id, type;
    int idx;
    size_t oldListSize = robotDataList.size();
    bool listChanged = false;

    // Split into individual data elements
    QStringList data = dataString.split(QRegExp("\\s+"));

    // Data packets must contain minimum of 3 elements; ID, type, content
    if (data.length() < 3) {
        cout << "Invalid data pakcet: " << dataString.toStdString() << endl;
        return;
    }

    // Try to obtain the ID from the first element
    bool ok;
    id = data[0].toInt(&ok, 10);

    if (!ok || id < 0) {
        cout << "Invalid robot ID: " << data[0].toStdString() << ", Data ignored." << endl;
        return;
    }

    // Try to obtain the packet type from the second element
    type = data[1].toInt(&ok, 10);

    if (!ok) {
        cout << "Invalid packet type: " << data[1].toStdString() << ", Data ignored." << endl;
        return;
    }

    // Get the list index of the given robot. New robot added if index not found
    idx = getRobotIndex(id);
    RobotData* robot = robotDataList.at(idx);

    // Handle the packet data
    switch(type) {
    case PACKET_TYPE_WATCHDOG:
        if (!(robot->getName() == data[2])) {
            robot->setName(data[2]);
            listChanged = true;
        }
        break;
    case PACKET_TYPE_STATE:
        robot->setState(data[2]);
        break;
    case PACKET_TYPE_POSITION:
        if (data.length() > 3) {
            parsePositionPacket(robot, data[2], data[3]);
        }
        break;
    default:
        break;
    }

    // Check if the list of robots has changed and needs updating
    if (robotDataList.size() != oldListSize) {
        listChanged = true;
    }

    // Signal to the UI that new data is available
    emit modelChanged(listChanged);
}

/* parsePositionPacket
 * Parses floating point data from the string elements of a position packet.
 */
void DataModel::parsePositionPacket(RobotData* robot, QString xString, QString yString) {
    bool ok;
    float x = xString.toFloat(&ok);

    if(!ok) {
        return;
    }

    float y = yString.toFloat(&ok);

    if(!ok) {
        return;
    }

    robot->setPos(x, y);
}

/* getRobotIndex
 * Finds the index of the robot with the given id. If create is set to true, and a robot with
 * the corresponding id is not found, one will be created. If not, -1 is returned to indicate that
 * the robot does not exist.
 */
int DataModel::getRobotIndex(int id, bool create) {
    bool found = false;
    int idx;

    // Check if the data is related to a known robot
    for (idx = 0; idx < (int)robotDataList.size(); idx++) {
        RobotData* d = (RobotData*)robotDataList.at(idx);

        if (d->getID() == id) {
            found = true;
            break;
        }
    }

    // Robot was not found
    if (!found) {
        // If create is false, return -1 to indicate robot not found.
        if (!create) {
            return -1;
        }

        // Add the robot to the list
        RobotData* newRobot = new RobotData(id, "Unknown");
        robotDataList.push_back(newRobot);

        // Sort
        sort(robotDataList.begin(), robotDataList.end(), sortRobotDataByID);

        // Use recursion to get the index
        idx = getRobotIndex(id);
    }

    // Return the index
    return idx;
}
