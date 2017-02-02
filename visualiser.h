#ifndef VISUALISER_H
#define VISUALISER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <opencv2/opencv.hpp>

class Visualiser : public QWidget
{
    Q_OBJECT
public:
    explicit Visualiser(QWidget *parent = 0);

    QSize minimumSizeHint () const {return QSize(200, 200); }

    void startVis(void);
    void stopVis(void);
    int isVisActive(void);

public slots:
    void showImage(const cv::Mat& image);
    void getImageFromCamera(void);

protected:
    void paintEvent(QPaintEvent*);

    QImage _qimage;
    cv::Mat _tmp;

    QTimer* cameraTimer;
};

#endif // VISUALISER_H
