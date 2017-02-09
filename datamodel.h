#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <vector>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include "robotdata.h"

#define PACKET_TYPE_WATCHDOG    0
#define PACKET_TYPE_STATE       1
#define PACKET_TYPE_POSITION    2

class DataModel : public QObject
{
    Q_OBJECT
    QStringListModel* robotListModel;
    std::vector<RobotData*> robotDataList;

public:
    explicit DataModel(QObject *parent = 0);
    ~DataModel(void);

    RobotData* getRobotByIndex(int idx);
    RobotData* getRobotByID(int id);

    QStringListModel* getRobotList(void);

    int getRobotIndex(int id, bool create = true);

private:
    void parsePositionPacket(RobotData* robot, QString xString, QString yString);

signals:
    void modelChanged(bool listChanged);

public slots:
    void newData(const QString &);
};

#endif // DATAMODEL_H
