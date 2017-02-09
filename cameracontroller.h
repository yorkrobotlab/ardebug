#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <opencv2/opencv.hpp>

#include "machinevision.h"

#include <QObject>
#include <QTimer>

class CameraController : public QObject
{
    Q_OBJECT
    MachineVision* machineVision;
    QTimer* readTimer;
    int frameSize = 10;
    bool cameraLoaded = false;

public:
    CameraController(void);
    ~CameraController(void);

public slots:
    void updateFrameSize(int size);
    void startReadingCamera(void);
    void stopReadingCamera(void);

private slots:
    void readCamera(void);

signals:
    void dataFromCamera(const cv::Mat &image);
    void posData(const QString &);
};

#endif // CAMERACONTROLLER_H
