#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <opencv2/opencv.hpp>

#include <QObject>
#include <QTimer>

class CameraController : public QObject
{
    Q_OBJECT
    QTimer* readTimer;
    int frameSize = 10;

public:
    CameraController(void);

public slots:
    void updateFrameSize(int size);
    void startReadingCamera(void);
    void stopReadingCamera(void);

private slots:
    void readCamera(void);

signals:
    void dataFromCamera(const cv::Mat &image);
};

#endif // CAMERACONTROLLER_H
