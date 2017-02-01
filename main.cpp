#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include <iostream>
#include <iomanip>
#include <vector>
#include <iCVCDriver.h>
#include <iCVCImg.h>
#include <iCVGenApi.h>
#include <CVCError.h>
#include <iCVCUtilities.h>

using namespace std;
using namespace cv;

static const size_t DRIVERPATHSIZE = 256;

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

int main(int argc, char *argv[])
{
    /*****Camera calibration parameters**********/
    /*****Done on 18/08/2016 02:40:21 PM*********/
    /*https://github.com/daneshtarapore/apriltags-cpp/blob/optimisation/out_camera_data.xml*/

    double tmp_cameraMatrix[3][3] = {{1.6349274125326908e+03, 0.0, 1.2795000000000000e+03}, {0.0, 1.6349274125326908e+03, 1.0235000000000000e+03}, {0.0, 0.0, 1}};
    cv::Mat cameraMatrix = Mat(3, 3, CV_64FC1, &tmp_cameraMatrix);
    double tmp_distCoeffs[5][1] = {-1.4790445043449291e-01, 7.8218565302159274e-02, 0.0, 0.0, -1.7646117392362880e-03};
    cv::Mat distCoeffs = Mat(5, 1, CV_64FC1, &tmp_distCoeffs);
    /********************************************/

    // Load the camera
    char driverPath[DRIVERPATHSIZE] = { 0 };
    TranslateFileName("%CVB%/drivers/GenICam.vin", driverPath, DRIVERPATHSIZE);
    IMG hCamera = NULL;

    bool success = LoadImageFile(driverPath, hCamera);

    if(!success)
    {
        cout << "Error loading " << driverPath << " driver!" << endl;
        return 1;
    }

    cout << "Load " << driverPath << " successful." << endl;

    // Start grab with ring buffer
    cvbres_t result = G2Grab(hCamera);

    int counter = 0;

    if(result >= 0)
    {
        // Create an attached OpenCV image
        Mat distorted_image = cvb_to_ocv_nocopy(hCamera);

        // Swap blue and red channels
        vector<Mat> channels(3);
        split(distorted_image, channels);
        merge(vector<Mat>{channels[2], channels[1], channels[0]}, distorted_image);

        // Undistort the image
        Mat image;
        undistort(distorted_image, image, cameraMatrix, distCoeffs);

        resize(image, image, Size(image.cols * 0.5, image.rows * 0.5), 0, 0, INTER_CUBIC);
        imshow("JAI GO-5000C-PGE", image);
    }

    // Show QT Application
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
