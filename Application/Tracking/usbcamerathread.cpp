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
            if(shouldRun)
                emit newVideoFrame(image);
        }
        else
        {
            yieldCurrentThread();
        }
    }
    captureDevice.release();
}

void USBCameraThread::quit()
{
    shouldRun = false;
}
