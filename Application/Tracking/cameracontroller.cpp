/* cameracontroller.cpp
 *
 * This class encapsulates the camera controller. The controller reads the
 * machine vision camera data into the system whilst running.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include <vector>

#include "cameracontroller.h"
#include "machinevision.h"
#include "../Core/log.h"

/* Constructor
 * Create the read timer and connect it to the read function.
 */
CameraController::CameraController(void) {
    // Instantiate the machine vision class which contains the camera routines
    machineVision = new MachineVision();

    // Set up the timer to read the camera
    readTimer = new QTimer(this);
    connect(readTimer, SIGNAL(timeout()), this, SLOT(readCamera()));

    // Arbitrary non-zero initial frame size
    frameSize.x = 10;
    frameSize.y = 10;
}

/* Destructor
 * Release all memory.
 */
CameraController::~CameraController(void) {
    delete machineVision;
    delete readTimer;
}

/* updateFrameSize
 * The visualiser frame size has changed, update the local frame size variable
 * to match.
 */
void CameraController::updateFrameSize(int width, int height) {
    frameSize.x = width;
    frameSize.y = height;
}

/* startReadingCamera
 * Start the timer for reading images from the machine vision camera.
 */
void CameraController::startReadingCamera(void) {
    if (!cameraLoaded) {
        cameraLoaded = machineVision->setupCamera();
    }

    readTimer->start(21);
}

/* stopReadingCamera
 * Stop the timer for reading images from the camera.
 */
void CameraController::stopReadingCamera(void) {
    readTimer->stop();
    cameraLoaded = false;
    machineVision->releaseCamera();
}

/* readCamera
 * Performs the actual camera reading, at the current frame size. Emits the
 * image acquired in a signal.
 */
void CameraController::readCamera(void) {
    // Pause the read timer
    readTimer->stop();

    // Create a vector to store tracking results
    std::vector<TrackResult> result;
    result.reserve(10);

    // Check the camera is currently loaded
    if (cameraLoaded) {
        // Retrieve the image and tracking data, and emit the image data signal
        emit dataFromCamera(machineVision->getLatestFrame(frameSize, &result));

        // Restart timer
        readTimer->start(21);
    } else {
        Log::instance()->logMessage("Camera not loaded, stopping read timer.", true);
    }

    // Iterate over all tracking results and emit position data signals as necessary
//    for (size_t i = 0; i < result.size(); i++) {
//        TrackResult res = (TrackResult)result.at(i);
//        QString json = "{ \"id\":\"";
//        json += res.id;
//        json += "\", \"pose\":{\"x\":";
//        json += QString::number(res.pose.position.x);
//        json += ",\"y\":";
//        json += QString::number(res.pose.position.y);
//        json += ",\"yaw\":";
//        json += QString::number(res.pose.orientation);
//        json += "}}";
//        emit posData(json);
//        emit posData(QString::number(res.id) + " 2 " + QString::number(res.pose.position.x) + " " + QString::number(res.pose.position.y) + " " + QString::number(res.pose.orientation));
//    }
}
