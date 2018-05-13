#ifndef ARUCO_H
#define ARUCO_H

#include <opencv2/aruco.hpp>
#include <map>
#include <vector>

#include <QString>
#include <QObject>

#include "Application/Core/util.h"
#include "Application/Tracking/camerathread.h"

class ArUco : public QObject
{
    Q_OBJECT

public:
    ArUco(std::map<int, QString>* idMapping, ARCameraThread* cameraThread);

public slots:
    void newImageReceived(cv::Mat& image);

signals:
    void newRobotPosition(QString id, Pose pose);

private:
    cv::Ptr<cv::aruco::Dictionary> possibleTags;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParameters;
    std::map<int, QString>* arucoToStringIdMapping;
    ARCameraThread* cameraThread;
};

#endif // ARUCO_H
