#include "aruco.h"
#include "Application/Core/util.h"
#include <QtMath>

#include <iostream>

ArUco::ArUco(std::map<int, QString>* idMapping, ARCameraThread* cameraThread)
{
    possibleTags = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_50);
    detectorParameters = cv::aruco::DetectorParameters::create();
    arucoToStringIdMapping = idMapping;
    this->cameraThread = cameraThread;
    connect(cameraThread, SIGNAL(newVideoFrame(cv::Mat&)), this, SLOT(newImageReceived(cv::Mat&)));
}

void ArUco::newImageReceived(cv::Mat& image)
{
//    disconnect(cameraThread, SIGNAL(newVideoFrame(cv::Mat&)), this, SLOT(newImageReceived(cv::Mat&)));
    std::cout<<"Aruco"<<std::endl<<std::flush;
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> detectedTags, junk;

    cv::aruco::detectMarkers(image, possibleTags, detectedTags, ids, detectorParameters, junk);

    double width = image.cols;
    double height = image.rows;

    for(unsigned int i = 0; i < ids.size(); ++i)
    {
        int id = ids[i];
        if(arucoToStringIdMapping->find(id) == arucoToStringIdMapping->end())
            continue;

        QString idString = (*arucoToStringIdMapping)[id];
        auto tag = detectedTags[i];

        cv::Point2f tagCentre = {0, 0};
        for(auto& corner : tag)
            tagCentre += corner * (1.0 / tag.size());

        cv::Point2f frontOfTag = 0.5 * (tag[0] + tag[1]);
        double orientation = qRadiansToDegrees(std::atan2(frontOfTag.y - tagCentre.y, frontOfTag.x - tagCentre.x));

        tagCentre.x /= width;
        tagCentre.y /= height;

        Pose p;
        p.orientation = orientation;
        p.position.x = tagCentre.x;
        p.position.y = tagCentre.y;

        emit newRobotPosition(idString, p);
    }

    cameraThread->addPreEmitCall([&](){
        connect(cameraThread, SIGNAL(newVideoFrame(cv::Mat&)), this, SLOT(newImageReceived(cv::Mat&)));
    });
}
