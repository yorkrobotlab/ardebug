#ifndef CVBCAMERATHREAD_H
#define CVBCAMERATHREAD_H

#include "../Core/appconfig.h"

#ifdef CVB_CAMERA_PRESENT

#include <opencv2/opencv.hpp>

#include <QThread>

#include <iCVCImg.h>

#include <functional>
#include <QMutex>
#include <QMutexLocker>

class CVBCameraThread : public QThread
{
    Q_OBJECT

public:
    CVBCameraThread();
    virtual void run() override;
    void addPreEmitCall(std::function<void()> call)
    {
        QMutexLocker lock{&emitCallMutex};
        preEmitCalls.push_back(call);
    }

public slots:
    virtual void quit();

signals:
    void newVideoFrame(cv::Mat& image);

private:
    IMG hCamera = NULL;
    volatile bool shouldRun = true;
    std::vector<std::function<void()>> preEmitCalls;
    QMutex emitCallMutex;
};

Q_DECLARE_METATYPE(cv::Mat)

#endif // CVB_CAMERA_PRESENT

#endif // CVBCAMERATHREAD_H
