#ifndef MACHINEVISION_H
#define MACHINEVISION_H

#include "../Core/util.h"
#include "../Core/appconfig.h"

#include <opencv2/opencv.hpp>

#ifdef CVB_CAMERA_PRESENT

#include <iCVCImg.h>

#endif /* CVB_CAMERA_PRESENT */

class MachineVision
{
private:

#ifdef CVB_CAMERA_PRESENT

    IMG hCamera = NULL;

#endif /* CVB_CAMERA_PRESENT */

public:
    bool setupCamera(void);
    cv::Mat getLatestFrame(Vector2D size, std::vector<TrackResult>* result);
    void releaseCamera(void);
};

#endif // MACHINEVISION_H
