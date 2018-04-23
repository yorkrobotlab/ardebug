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

public slots:
    void endThread();

signals:
    void newVideoFrame(cv::Mat& image);

private:
    cv::VideoCapture captureDevice;
    volatile bool shouldRun = true;
};

Q_DECLARE_METATYPE(cv::Mat)

#endif // USBCAMERATHREAD_H
