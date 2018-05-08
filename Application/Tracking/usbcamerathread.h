#ifndef USBCAMERATHREAD_H
#define USBCAMERATHREAD_H

#include <opencv2/videoio.hpp>

#include <QThread>

class USBCameraThread : public QThread
{
    Q_OBJECT

public:
    USBCameraThread();
    virtual void run() override;

public slots:
    virtual void quit();

signals:
    void newVideoFrame(cv::Mat& image);

private:
    cv::VideoCapture captureDevice;
    volatile bool shouldRun = true;
};

Q_DECLARE_METATYPE(cv::Mat)

#endif // USBCAMERATHREAD_H
