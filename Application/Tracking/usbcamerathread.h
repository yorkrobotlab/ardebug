#ifndef USBCAMERATHREAD_H
#define USBCAMERATHREAD_H

#include <opencv2/videoio.hpp>

#include <QThread>

class USBCameraThread : public QThread
{
    Q_OBJECT

public:
    USBCameraThread(QString deviceName);
    virtual void run() override;

signals:
    void newVideoFrame(cv::Mat& image);

private:
    cv::VideoCapture captureDevice;
    bool shouldRun = true;
};

Q_DECLARE_METATYPE(cv::Mat)

#endif // USBCAMERATHREAD_H
