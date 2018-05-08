#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H


#include <QObject>
#include <QDialog>
#include <QListView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include "../DataModel/datamodel.h"
#include <QString>
#include <QtCharts/QChartView>



class Chartdialog : public QDialog
{
private:
    Q_OBJECT
public:
    explicit Chartdialog(DataModel* data);
    ~Chartdialog();
private:
    DataModel* data;
    QtCharts::QChart *chart;


private slots:


public slots:
    void newDataSelected(const QString &);


};




#endif // CHARTDIALOG_H

