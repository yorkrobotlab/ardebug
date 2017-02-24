#ifndef IRDATAVIEW_H
#define IRDATAVIEW_H

#include "datamodel.h"

#include <QWidget>

class IRDataView : public QWidget
{
    Q_OBJECT
public:
    explicit IRDataView(QWidget *parent = 0);
    IRDataView(DataModel* dataModelRef);

    void paintEvent(QPaintEvent *event);

signals:

public slots:

protected:
    DataModel* dataModelRef;
};

#endif // IRDATAVIEW_H
