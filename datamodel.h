#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>

class DataModel : public QObject
{
    Q_OBJECT
    QStringListModel* robotListModel;

public:
    explicit DataModel(QObject *parent = 0);

    QStringListModel* getRobotList(void);

signals:
    void modelChanged(void);

public slots:
    void newData(const QString &);
};

#endif // DATAMODEL_H
