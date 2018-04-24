#include "usbcamerathread.h"

#include <iostream>

USBCameraThread::USBCameraThread(QString deviceName)
{
    captureDevice = cv::VideoCapture(0);

    if(captureDevice.isOpened())
        std::cout << "Successfully opened capture device" << std::endl;
    else
    {
        std::cout << "Failed to open capture device" << std::endl;
        exit(0);
    }
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
