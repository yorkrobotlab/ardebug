#ifndef MACHINEVISION_H
#define MACHINEVISION_H

#include <appconfig.h>

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
    cv::Mat getLatestFrame(int size);
    void releaseCamera(void);
};

#endif // MACHINEVISION_H
