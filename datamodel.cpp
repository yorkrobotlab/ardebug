/* datamodel.cpp
 *
 * This class encapsulates the data model used to store data on the active
 * robots.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "datamodel.h"
#include "util.h"
#include "log.h"
#include "settings.h"

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

    // Initially no robot selected
    selectedRobotID = -1;
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
    int id, type;
    int idx;
    size_t oldListSize = robotDataList.size();
    bool listChanged = false;

    // Split into individual data elements
    QStringList data = dataString.split(QRegExp("\\s+"));

    // Data packets must contain minimum of 3 elements; ID, type, content
    if (data.length() < 3) {
        Log::instance()->logMessage("Invalid data pakcet: " + dataString, true);
        return;
    }

    // Try to obtain the ID from the first element
    bool ok;
    id = data[0].toInt(&ok, 10);

    if (!ok || id < 0) {
        Log::instance()->logMessage("Invalid robot ID: " + data[0] + ", Data ignored.", true);
        return;
    }

    // Try to obtain the packet type from the second element
    type = data[1].toInt(&ok, 10);

    if (!ok || type < 0 || type >= PACKET_TYPE_INVALID) {
        Log::instance()->logMessage("Invalid packet type: " + data[1] + ", Data ignored.", true);
        return;
    }

    /* If this is a position packet the ID will be an ARuCo ID. Check the ID mapping table to see if
     * this needs to be mapped to a different robot ID.
     */
    if (type == PACKET_TYPE_POSITION) {
        for (size_t i = 0; i < Settings::instance()->idMapping.size(); i++) {
            ArucoIDPair* pair = Settings::instance()->idMapping.at(i);

            // If there is a match in the map, swap for the correct robot ID
            if (pair->arucoID == id) {
                id = pair->robotID;
                break;
            }
        }
    }

    // Get the list index of the given robot. New robot added if index not found
    idx = getRobotIndex(id, true);
    RobotData* robot = robotDataList.at(idx);

    // Handle the packet data
    switch(type) {
    case PACKET_TYPE_WATCHDOG:
        if (!(robot->getName() == data[2])) {
            robot->setName(data[2]);
            listChanged = true;
        }
        Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + " - Watchdog Packet", false);
        break;
    case PACKET_TYPE_STATE:
        robot->setState(data[2]);
        Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + " - State " + data[2], false);
        break;
    case PACKET_TYPE_POSITION:
        if (data.length() > 4) {
            parsePositionPacket(robot, data[2], data[3], data[4]);
            //Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + ": Position X:" + data[2] + ", Y:" + data[3] + ", A:" + data[4], false);
        }
        break;
    case PACKET_TYPE_PROXIMITY:
        parseProximityPacket(robot, data, false);
        break;
    case PACKET_TYPE_BACKGROUND_IR:
        parseProximityPacket(robot, data, true);
        break;
    case PACKET_TYPE_MSG:
        data.removeFirst();
        data.removeFirst();
        Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + " - Message: " + data.join(" "), true);
    case PACKET_TYPE_CUSTOM:
        if (data.length() > 3) {
            robot->insertCustomData(data[2], data[3]);
            Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + " - Custom Data: " + data[2] + " " + data[3], false);
        }
    default:
        break;
    }

    // Check if the list of robots has changed size and needs updating
    if (robotDataList.size() != oldListSize) {
        listChanged = true;
    }

    // Signal to the UI that new data is available
    emit modelChanged(listChanged);
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
        Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + ": Background IR Data: " + data.join(" "), false);
    } else {
        Log::instance()->logMessage("Robot " + QString::number(robot->getID()) + ": IR Data: " + data.join(" "), false);
    }
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

void DataModel::resetRobotColours(void) {
    for (int i = 0; i < (int)robotDataList.size(); i++) {
        RobotData* d = (RobotData*)robotDataList.at(i);
        d->setColour(colourGen());
    }
}
