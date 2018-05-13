#ifndef USBCAMERATHREAD_H
#define USBCAMERATHREAD_H

#include <opencv2/videoio.hpp>

#include "camerathread.h"

class USBCameraThread : public ARCameraThread
{
    Q_OBJECT

public:
    USBCameraThread();
    virtual void run() override;

signals:
    void newVideoFrame(cv::Mat& image);

private:
    cv::VideoCapture captureDevice;
};

#endif // USBCAMERATHREAD_H
