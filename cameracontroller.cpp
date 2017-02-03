/* cameracontroller.cpp
 *
 * This class encapsulates the camera controller. The controller reads the
 * machine vision camera data into the system whilst running.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "cameracontroller.h"

#include "machinevision.h"

/* Constructor
 * Create the read timer and connect it to the read function.
 */
CameraController::CameraController(void) {
    readTimer = new QTimer(this);
    connect(readTimer, SIGNAL(timeout()), this, SLOT(readCamera()));
}

/* updateFrameSize
 * The visualiser frame size has changed, update the local frame size variable
 * to match.
 */
void CameraController::updateFrameSize(int size) {
    frameSize = size;
}

/* startReadingCamera
 * Start the timer for reading images from the machine vision camera.
 */
void CameraController::startReadingCamera(void) {
    readTimer->start(41);
}

/* stopReadingCamera
 * Stop the timer for reading images from the camera.
 */
void CameraController::stopReadingCamera(void) {
    readTimer->stop();
}

/* readCamera
 * Performs the actual camera reading, at the current frame size. Emits the
 * image acquired in a signal.
 */
void CameraController::readCamera(void) {
    readTimer->stop();
    emit dataFromCamera(machineVision_getLatestFrame(frameSize));
    readTimer->start(41);
}
