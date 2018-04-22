#ifndef VISUALISER_H
#define VISUALISER_H

#include "../DataModel/datamodel.h"
#include "visconfig.h"
#include "../Core/util.h"

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>

class Visualiser : public QWidget
{
    Q_OBJECT
public:
    VisConfig config;

    Visualiser(QWidget *parent = 0);
    Visualiser(DataModel* dataModelRef);

    QSize minimumSizeHint () const { return QSize(200, 200); }

    void checkFrameSize(void);

public slots:
    void refreshVisualisation();

signals:
    void frameSizeChanged(int width, int height);
    void robotSelectedInVisualiser(QString id);

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);

    void mousePressEvent(QMouseEvent*);

    DataModel* dataModelRef;

    Vector2D click;
};

#endif // VISUALISER_H
