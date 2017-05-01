/* machinevision.cpp
 *
 * Functions related to the machine vision camera. The CVB_CAMERA_PRESENT
 * define is used to allow compilation on machines other than the tracking
 * server, where the camera libraries and drivers are not present.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "machinevision.h"
#include "settings.h"
#include "log.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>

#include <opencv2/aruco.hpp>

#ifdef CVB_CAMERA_PRESENT

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

    return image;
}

/* projectpoint_image_to_world
 * Taken from original tracking code. Converts point in image space to a
 * point in world space.
 */
void projectpoint_image_to_world(cv::Mat cameraMatrix, double world_z, cv::Point_<float> undistort_imgpoint, cv::Point_<float> &undistort_worldpoint)
{
    double image_x = undistort_imgpoint.x;
    double image_y = undistort_imgpoint.y;

    double c_x = cameraMatrix.at<double>(0,2);
    double c_y = cameraMatrix.at<double>(1,2);

    double f_x = cameraMatrix.at<double>(0,0);
    double f_y = cameraMatrix.at<double>(1,1);

    /*
     * ref: http://stackoverflow.com/questions/12007775/to-calculate-world-coordinates-from-screen-coordinates-with-opencv
     x_screen = (x_world/z_world)*f_x + c_x
     y_screen = (y_world/z_world)*f_y + c_y

     x_world = (x_screen - c_x) * z_world / f_x
     y_world = (y_screen - c_y) * z_world / f_y*/

    double world_x = (image_x - c_x) * world_z / f_x;
    double world_y = (image_y - c_y) * world_z / f_y;

    undistort_worldpoint.x = world_x;
    undistort_worldpoint.y = world_y;

    /*
     * But from my calculations, I think we should use the following:
    double world_x = (image_x - c_x * world_z) / f_x;
    double world_y = (image_y - c_y * world_z) / f_y;*/
}

/* setupCamera
 * Set up the camera data.
 */
bool MachineVision::setupCamera(void) {
    // Load the camera
    char driverPath[DRIVERPATHSIZE] = { 0 };
    TranslateFileName("%CVB%/drivers/GenICam.vin", driverPath, DRIVERPATHSIZE);

    bool success = LoadImageFile(driverPath, hCamera);

    if(!success)
    {
        Log::instance()->logMessage("Error loading " + QString(driverPath) + " driver!", true);
        return false;
    }

    Log::instance()->logMessage("Load " + QString(driverPath) + " successful.", true);

    // Start grab with ring buffer
    cvbres_t result = G2Grab(hCamera);

    if(result < 0)
    {
        Log::instance()->logMessage("Error grabbing camera with ring buffer.", true);
        return false;
    }

    return true;
}

/* getLatestFrame
 * Gets the latest video frame.
 */
Mat MachineVision::getLatestFrame(Vector2D size, std::vector<TrackResult>* result) {
    /*****Camera calibration parameters**********/
    /*****Done on 18/08/2016 02:40:21 PM*********/
    /*https://github.com/daneshtarapore/apriltags-cpp/blob/optimisation/out_camera_data.xml*/

    double tmp_cameraMatrix[3][3] = {{1.6349274125326908e+03, 0.0, 1.2795000000000000e+03}, {0.0, 1.6349274125326908e+03, 1.0235000000000000e+03}, {0.0, 0.0, 1}};
    Mat cameraMatrix = Mat(3, 3, CV_64FC1, &tmp_cameraMatrix);
    double tmp_distCoeffs[5][1] = {-1.4790445043449291e-01, 7.8218565302159274e-02, 0.0, 0.0, -1.7646117392362880e-03};
    Mat distCoeffs = Mat(5, 1, CV_64FC1, &tmp_distCoeffs);
    /********************************************/

    Mat image;

    // Wait for next image to be acquired
    // (returns immediately if unprocessed images are in the ring buffer)
    cvbres_t camResult = G2Wait(hCamera);

    if(camResult < 0) {
        Log::instance()->logMessage("Error with G2Wait: " + QString::number(CVC_ERROR_FROM_HRES(camResult)), true);
        image = Mat(size.x, size.y, CV_8UC3);
    } else {
        // Create an attached OpenCV image
        Mat originalImage = cvb_to_ocv_nocopy(hCamera);

        Mat unflippedImage;
        originalImage.convertTo(unflippedImage, -1, 2, 40);

        // Swap blue and red channels
        vector<Mat> channels(3);
        split(unflippedImage, channels);
        merge(vector<Mat>{channels[2], channels[1], channels[0]}, unflippedImage);

        if (Settings::instance()->isImageFlipped()) {
            flip(unflippedImage, image, -1);
        } else {
            image = unflippedImage;
        }


        // Create arrays for aruco results
        vector<int> marker_ids;
        vector<vector<Point2f> > marker_corners, rejected_candidates;

        // Generate an aruco dictionary (Why do this every time?)
        Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();
        Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_50);

        // Detect ArUco markers
        aruco::detectMarkers(image, dictionary, marker_corners, marker_ids, parameters, rejected_candidates);

        // Create tracking results from detected marker positions
        for (size_t i = 0; i < marker_ids.size(); i++) {
            TrackResult t;
            t.id = marker_ids.at(i);

            // Interpolate for center
            Point2f marker_centre = (marker_corners[i][0] +
                                     marker_corners[i][1] +
                                     marker_corners[i][2] +
                                     marker_corners[i][3]) / 4;

            // Result positions are proportional
            t.pos.x = marker_centre.x / image.cols;
            t.pos.y = marker_centre.y / image.rows;

            double bottom_line_angle = atan2(marker_corners[i][2].y - marker_corners[i][3].y,
                                             marker_corners[i][2].x - marker_corners[i][3].x)
                                       * 180.0f / M_PI; // atan2 returns angle in radians [-pi, pi]

            // Ensure angle is a valid number
            if(std::isnan(bottom_line_angle))
            {
                t.angle = 0;
            } else {
                t.angle = int(ceil(bottom_line_angle - 90));
            }

            // Stash this result
            result->push_back(t);
        }

        // Undistort the image
        //undistort(distorted_image, image, cameraMatrix, distCoeffs);

        // Resize Image
        resize(image, image, Size(size.x, size.y), 0, 0, INTER_CUBIC);
    }

    if (Settings::instance()->isVideoEnabled()) {
        return image;
    } else {
        return Mat(size.y, size.x, CV_8UC3, Scalar(0, 0, 0));
    }
}

/* releaseCamera
 * Release the camera data.
 */
void MachineVision::releaseCamera(void) {
    ReleaseObject(hCamera);
}

#else /* CVB_CAMERA_PRESENT */

#define PI 3.14159265

using namespace std;
using namespace cv;

/* setupCamera
 * Set up the camera data.
 */
bool MachineVision::setupCamera(void) {
    return true;
}

/* getLatestFrame
 * Gets the latest video frame.
 */
Mat MachineVision::getLatestFrame(Vector2D size, std::vector<TrackResult>* result) {
    // Time varying value
    static int r = 0;
    r+=1;
    if(r>359) r=0;

    // Fake some robot position and angle data
    TrackResult res;
    res.id = 1;
    res.pos.x = 0.5 + (0.2 * (float)cos(r * (PI/180)));
    res.pos.y = 0.5 + (0.2 * (float)sin(r * (PI/180)));
    res.angle = (r > 269) ? r + -270 : r + 90;
    result->push_back(res);

    res.id = 2;
    res.pos.x = 0.5 + (0.3 * (float)sin(r * (PI/180)));
    res.pos.y = 0.5 + (0.3 * (float)cos(r * (PI/180)));
    res.angle = 360 - r;
    result->push_back(res);

    res.id = 3;
    res.pos.x = 0.5 + (0.1 * (float)sin(r * (PI/180)));
    res.pos.y = 0.5 + (0.1 * (float)cos(r * (PI/180)));
    res.angle = 360 - r;
    result->push_back(res);

    res.id = 4;
    res.pos.x = 0.5 + (0.45 * (float)cos(r * (PI/180)));
    res.pos.y = 0.5 + (0.45 * (float)sin(r * (PI/180)));
    res.angle = (r > 269) ? r + -270 : r + 90;
    result->push_back(res);

    res.id = 5;
    res.pos.x += + (0.1 * (float)cos(4 * r * (PI/180)));
    res.pos.y += (0.1 * (float)sin(4 * r * (PI/180)));
    res.angle = 360 - (4 * r);
    result->push_back(res);

    // Camera is not present, fake the frame
    if (Settings::instance()->isVideoEnabled()) {
        return Mat(size.y, size.x, CV_8UC3, Scalar(192, 192, 192));
    } else {
        return Mat(size.y, size.x, CV_8UC3, Scalar(0, 0, 0));
    }
}

/* releaseCamera
 * Release the camera data.
 */
void MachineVision::releaseCamera(void) {
    // Do Nothing
}

#endif /* CVB_CAMERA_PRESENT */
