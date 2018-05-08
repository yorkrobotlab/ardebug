
/* bluetoothconfigdialog.cpp
 *
 * This class encapsulates a small dialog for adding a new tag ID -> robot ID mapping.
 *
 * Charlotte Arndt Feb 2018
 */

#include "chartdialog.h"
#include <QFormLayout>
#include <QPushButton>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QString>

/* constructor
 *
 */
Chartdialog::Chartdialog(DataModel* data)
{

    this->setModal(false);
    this->setGeometry(200, 200, 600, 400);
    this->setWindowTitle("Dataview");
    this->data = data;



    chart = new QtCharts::QChart();

    chart->setTitle("robot data");
    chart->legend()->hide();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    // Arrange the rows vertically
    QVBoxLayout* mainbox = new QVBoxLayout();

    mainbox->addWidget(chartView);

    // Set layout
    this->setLayout(mainbox);
}

/* deconstructor
 * frees memory
 */
Chartdialog::~Chartdialog()
{


}

  void Chartdialog::newDataSelected(const QString & dataset)
  {
    QMap<QString, int> entryList;

    chart->removeAllSeries();
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    series->setLabelsVisible(true);



    QString selectedRobotID = "1"; //data->selectedRobotID();

    for(int i = 0; i<data->getRobotCount(); i++)
    {
        RobotData* robot = data->getRobotByIndex(i);
        if (robot->getID() == selectedRobotID)
        {


        }
        QString value = robot->getStringValue(dataset);

        if (entryList.contains(value))
        {
           entryList[value] = entryList[value] + 1;

        }
        else
        {
           entryList[value] = 1;

        }
        qDebug()<<"robot: " <<i <<  " value: "<< value;


    }
    int counter = 0;
    for(const auto& key : entryList.keys())
    {
        series->append(key, entryList[key]);
        QtCharts::QPieSlice *slice = series->slices().at(counter );

         slice->setLabelVisible();

        qDebug()<<"data in dialog: " <<key;
        counter ++;

    }


    chart->addSeries(series);

     // qDebug()<<"data in dialog: " <<dataset;


  }





