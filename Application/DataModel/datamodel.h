#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <vector>
#include <functional>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include <QMutex>
#include <QMutexLocker>
#include <QTimer>

#include "robotdata.h"

#define PACKET_TYPE_WATCHDOG        0
#define PACKET_TYPE_STATE           1
#define PACKET_TYPE_POSITION        2
#define PACKET_TYPE_PROXIMITY       3
#define PACKET_TYPE_BACKGROUND_IR   4
#define PACKET_TYPE_MSG             5
#define PACKET_TYPE_CUSTOM          6
#define PACKET_TYPE_INVALID         7

class DataModel : public QObject
{
    Q_OBJECT
    QStringListModel* robotListModel;
    std::vector<RobotData*> robotDataList;
    QTimer newDataTimer{this};

public:
    QString selectedRobotID;

    Vector2D averageRobotPos;

    explicit DataModel(QObject *parent = 0);
    ~DataModel(void);

    RobotData* getRobotByID(QString id);
    RobotData* getRobotByIndex(int idx) { return robotDataList[idx]; }

    QStringListModel* getRobotList(void);

    int getRobotCount(void);
    RobotData* setSelectedRobot(int idx);

    void sort(std::function<int(RobotData*,RobotData*)> sortFunc = nullptr);

private:
    void parsePositionPacket(RobotData* robot, QString xString, QString yString, QString aString);
    void parseProximityPacket(RobotData* robot, QStringList data, bool background);
    void updateAveragePosition(void);
    void addRobotIfNotExist(QString id);

signals:
    void modelChanged(bool listChanged, QString robotId, std::vector<QString> changedData);

public slots:
    void newData(const QString &);
    void deleteRobot(QString ID);
    void newRobotPosition(QString, Pose);
    void emitModelChangedSignal();
};

#endif // DATAMODEL_H
