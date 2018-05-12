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
            {
                executePreEmitCalls();
                emit newVideoFrame(image);
                disconnect(this, SIGNAL(newVideoFrame(cv::Mat&)), nullptr, 0);
            }
        }
        else
        {
            yieldCurrentThread();
        }
    }
    captureDevice.release();
}
