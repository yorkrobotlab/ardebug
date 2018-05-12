#include "cvbcamerathread.h"

#ifdef CVB_CAMERA_PRESENT

#include <iostream>
#include <iomanip>

#include <iCVCDriver.h>
#include <iCVGenApi.h>
#include <CVCError.h>
#include <iCVCUtilities.h>

using namespace std;
using namespace cv;

static const size_t DRIVERPATHSIZE = 256;

/* cvb_to_ocv_nocopy
 * Taen from original tracking code. Gets the image from the CVB camera
 * and converts it to an opencv image.
 */
Mat cvb_to_ocv_nocopy(IMG cvbImg)
{
    // Construct an appropriate OpenCV image
    Size size(ImageWidth(cvbImg), ImageHeight(cvbImg));
    void* ppixels = nullptr;
    intptr_t xInc = 0;
    intptr_t yInc = 0;
    GetLinearAccess(cvbImg, 0, &ppixels, &xInc, &yInc);
    Mat image(size, CV_8UC3, ppixels, yInc);
    Mat ret;

    cv::cvtColor(image, ret, CV_BGR2RGB);

    return ret;
}

CVBCameraThread::CVBCameraThread()
{
    cout << "CVBCameraThread constructor called" << endl;

    // Load the camera
    char driverPath[DRIVERPATHSIZE] = { 0 };
    TranslateFileName("%CVB%/drivers/GenICam.vin", driverPath, DRIVERPATHSIZE);

    bool success = LoadImageFile(driverPath, hCamera);

    if(!success)
    {
        cout << "Error loading " << driverPath << " driver!" << endl;
        exit(0);
    }

    cout << "Load " << driverPath << " successful." << endl;

    // Start grab with ring buffer
    cvbres_t result = G2Grab(hCamera);

    if(result < 0)
    {
        cout << "Error grabbing camera with ring buffer." << endl;
        exit(0);
    }
}

void CVBCameraThread::run()
{
    while(shouldRun)
    {
        Mat image;

        // Wait for next image to be acquired
        // (returns immediately if unprocessed images are in the ring buffer)
        cvbres_t camResult = G2Wait(hCamera);

        if(camResult < 0) {
            cout << setw(3) << "Error with G2Wait: " << CVC_ERROR_FROM_HRES(camResult) << endl;
        } else {
            // Create an attached OpenCV image
            Mat originalImage = cvb_to_ocv_nocopy(hCamera);
            originalImage.convertTo(originalImage, -1, 2, 0);

            if(shouldRun)
            {
                executePreEmitCalls();
                emit newVideoFrame(originalImage);
                disconnect(this, SIGNAL(newVideoFrame(cv::Mat&)), nullptr, 0);
            }
        }
    }

    G2Freeze(hCamera, true);
    ReleaseObject(hCamera);
}

#endif // CVB_CAMERA_PRESENT
