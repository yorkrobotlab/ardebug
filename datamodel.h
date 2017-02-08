#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <vector>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include "robotdata.h"

class DataModel : public QObject
{
    Q_OBJECT
    QStringListModel* robotListModel;
    std::vector<RobotData> robotDataList;

public:
    explicit DataModel(QObject *parent = 0);
    ~DataModel(void);

    QStringListModel* getRobotList(void);

signals:
    void modelChanged(void);

public slots:
    void newData(const QString &);
};

#endif // DATAMODEL_H
