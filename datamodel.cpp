#include "datamodel.h"

#include <stdio.h>
#include <iostream>

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    // Instantiate the data model here

    // Create a  model for the list of active robots
    robotListModel = new QStringListModel();

    // Create a string list of robots
    QStringList robotList;
    robotList.append("Robot 1");
    robotList.append("Robot 2");
    robotList.append("Robot 3");

    // Apply the list to the model and the model to the view
    robotListModel->setStringList(robotList);
}

QStringListModel* DataModel::getRobotList(void) {
    return robotListModel;
}

void DataModel::newData(const QString &data) {
    // Process the new data
    QStringList robotList = robotListModel->stringList();
    robotList.append(data.simplified());
    robotListModel->setStringList(robotList);

    // Signal to the UI that new data is available
    emit modelChanged();
}
