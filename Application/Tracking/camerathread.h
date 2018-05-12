#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <opencv2/videoio.hpp>

#include <QThread>
#include <functional>
#include <QMutex>
#include <QMutexLocker>

class ARCameraThread : public QThread
{
    Q_OBJECT

public:
    ARCameraThread() {}
    virtual void run() = 0;

    void addPreEmitCall(std::function<void()> call)
    {
        QMutexLocker lock{&emitCallMutex};
        preEmitCalls.push_back(call);
    }

public slots:
    virtual void quit() { this->blockSignals(true); shouldRun = false; }

signals:
    void newVideoFrame(cv::Mat& image);

protected:
    void executePreEmitCalls()
    {
        QMutexLocker lock{&emitCallMutex};
        for(auto f : preEmitCalls)
            f();
        preEmitCalls.clear();
    }
    volatile bool shouldRun = true;

private:
    std::vector<std::function<void()>> preEmitCalls;
    QMutex emitCallMutex;
};

Q_DECLARE_METATYPE(cv::Mat)

#endif // CAMERATHREAD_H
