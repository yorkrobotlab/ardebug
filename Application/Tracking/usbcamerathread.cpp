#include "usbcamerathread.h"

#include <iostream>

USBCameraThread::USBCameraThread(QString deviceName)
{
    std::cout<<"Opening capture device "<< (captureDevice.open(deviceName.toStdString(), cv::CAP_V4L) ? "SUCCESS" : "FAIL") <<std::endl;
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
