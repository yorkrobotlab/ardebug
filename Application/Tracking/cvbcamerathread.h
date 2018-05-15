#ifndef CVBCAMERATHREAD_H
#define CVBCAMERATHREAD_H

#ifdef CVB_CAMERA_PRESENT

#include <opencv2/opencv.hpp>

#include "camerathread.h"

#include <iCVCImg.h>

class CVBCameraThread : public ARCameraThread
{
    Q_OBJECT

public:
    CVBCameraThread();
    virtual void run() override;

signals:
    void newVideoFrame(cv::Mat& image);

private:
    IMG hCamera = NULL;
};

#endif // CVB_CAMERA_PRESENT

#endif // CVBCAMERATHREAD_H
