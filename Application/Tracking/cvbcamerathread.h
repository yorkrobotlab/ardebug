#ifndef CVBCAMERATHREAD_H
#define CVBCAMERATHREAD_H

#include "../Core/appconfig.h"

#ifdef CVB_CAMERA_PRESENT

#include <opencv2/opencv.hpp>

#include <QThread>

#include <iCVCImg.h>

class CVBCameraThread : public QThread
{
    Q_OBJECT

public:
    CVBCameraThread();
    virtual void run() override;

public slots:
    virtual void quit();

signals:
    void newVideoFrame(cv::Mat& image);

private:
    IMG hCamera = NULL;
    volatile bool shouldRun = true;
};

Q_DECLARE_METATYPE(cv::Mat)

#endif // CVB_CAMERA_PRESENT

#endif // CVBCAMERATHREAD_H
