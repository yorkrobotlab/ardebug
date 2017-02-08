#include "datamodel.h"

using namespace  std;

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    // Instantiate the data model here
    robotDataList.reserve(10);

    // Create a  model for the list of active robot ids
    robotListModel = new QStringListModel();
}

DataModel::~DataModel(void) {
    delete robotListModel;
}

QStringListModel* DataModel::getRobotList(void) {
    QStringList list;
    for(int i = 0; i < robotDataList.size(); i++) {
        RobotData* d = (RobotData*)robotDataList.at(i);
        list.append(d->getID());
    }

    robotListModel->setStringList(list);
    return robotListModel;
}

void DataModel::newData(const QString &data) {
    // Process the new data
    RobotData* robot = new RobotData(data.simplified());
    robotDataList.push_back(robot);

    // Signal to the UI that new data is available
    emit modelChanged();
}
