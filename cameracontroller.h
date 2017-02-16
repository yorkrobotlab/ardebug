#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <opencv2/opencv.hpp>

#include "machinevision.h"
#include "util.h"

#include <QObject>
#include <QTimer>

class CameraController : public QObject
{
    Q_OBJECT
    MachineVision* machineVision;
    QTimer* readTimer;
    Vector2D frameSize;
    bool cameraLoaded = false;

public:
    CameraController(void);
    ~CameraController(void);

public slots:
    void updateFrameSize(int width, int height);
    void startReadingCamera(void);
    void stopReadingCamera(void);

private slots:
    void readCamera(void);

signals:
    void dataFromCamera(const cv::Mat &image);
    void posData(const QString &);
};

#endif // CAMERACONTROLLER_H
