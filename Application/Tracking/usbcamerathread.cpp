#include "usbcamerathread.h"

#include <iostream>

USBCameraThread::USBCameraThread()
{
    captureDevice = cv::VideoCapture(0);
}

void USBCameraThread::run()
{
    while(shouldRun)
    {
        if(captureDevice.isOpened())
        {
            cv::Mat image;
            captureDevice>>image;
            emit newVideoFrame(image);
        }
        else
        {
            yieldCurrentThread();
        }
    }
}

void USBCameraThread::endThread()
{
    shouldRun = false;
}
