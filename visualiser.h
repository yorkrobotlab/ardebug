#ifndef VISUALISER_H
#define VISUALISER_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>

class Visualiser : public QWidget
{
    Q_OBJECT
public:
    explicit Visualiser(QWidget *parent = 0) : QWidget(parent) {}

    QSize sizeHint() const { return _qimage.size(); }
    QSize minimumSizeHint () const {return _qimage.size(); }

public slots:
    void showImage(const cv::Mat& image);

protected:
    void paintEvent(QPaintEvent*);

    QImage _qimage;
    cv::Mat _tmp;
};

#endif // VISUALISER_H
