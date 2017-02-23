/* settings.cpp
 *
 * This class encapsulates the settings singleton class.
 *
 * (C) Alistair Jewers Feb 2017
 */
#include "settings.h"

/* Constructor
 * Set defaults.
 */
Settings::Settings(void) {
    cameraImageSize.x = 2096;
    cameraImageSize.y = 1180;

    videoEnabled = true;
}

/* getCameraImageSize
 * Returns the camera image size setting.
 */
Vector2D Settings::getCameraImageSize(void) {
    return cameraImageSize;
}

/* setCameraImageSize
 * Sets the camera image size setting.
 */
void Settings::setCameraImageSize(Vector2D size) {
    this->cameraImageSize = size;

    if (size.x <= 0) {
        this->cameraImageSize.x = 1;
    }

    if (size.y <= 0) {
        this->cameraImageSize.y = 1;
    }
}

/* setCameraImageWidth
 * Set the camera image width value.
 */
void Settings::setCameraImageWidth(int width) {
    this->cameraImageSize.x = width > 0 ? width : 1;
}

/* setCameraImageHeight
 * Set the camera image height value.
 */
void Settings::setCameraImageHeight(int height) {
    this->cameraImageSize.y = height > 0 ? height : 1;
}

/* isVideoEnabled
 * Returns the boolean state of the video enabled setting.
 */
bool Settings::isVideoEnabled(void) {
    return this->videoEnabled;
}

/* setVideoEnabled
 * Enables or disables te video enabled setting.
 */
void Settings::setVideoEnabled(bool enable) {
    this->videoEnabled = enable;
}
