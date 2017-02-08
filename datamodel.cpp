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
bool sortRobotDataByID(const RobotData &lhs, const RobotData &rhs) {
    return lhs.getIDConst() < rhs.getIDConst();
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

/* getRobotList
 * Retrieves a QStringList of all the robots being tracked.
 */
QStringListModel* DataModel::getRobotList(void) {
    QStringList list;
    for(size_t i = 0; i < robotDataList.size(); i++) {
        RobotData d = (RobotData)robotDataList.at(i);
        QString str(QString::number(d.getID()) + ": " + d.getName());
        list.append(str);
    }

    robotListModel->setStringList(list);
    return robotListModel;
}

/* newData
 * Slot. Called when new data arrives.
 */
void DataModel::newData(const QString &dataString) {
    QStringList data = dataString.split(QRegExp("\\s+"));

    bool ok;
    int id = data[0].toInt(&ok, 10);

    if (!ok) {
        cout << "Invalid robot ID " << data[0].toStdString() << ", Data ignored." << endl;
        return;
    }

    for (size_t i = 0; i < robotDataList.size(); i++) {
        RobotData d = (RobotData)robotDataList.at(i);

        if (d.getID() == id) {
            // Matching, do nothing
            return;
        }
    }

    // Process the new data
    if (data.length() > 1) {
        RobotData robot = RobotData(id, data[1]);
        robotDataList.push_back(robot);
        sort(robotDataList.begin(), robotDataList.end(), sortRobotDataByID);

        // Signal to the UI that new data is available
        emit modelChanged();
    }
}
