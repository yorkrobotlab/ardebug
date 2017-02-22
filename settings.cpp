#include "settings.h"

Settings::Settings(void) {
    cameraImageSize.x = 2096;
    cameraImageSize.y = 1180;

    videoEnabled = true;
}

Vector2D Settings::getCameraImageSize(void) {
    return cameraImageSize;
}

void Settings::setCameraImageSize(Vector2D size) {
    this->cameraImageSize = size;

    if (size.x <= 0) {
        this->cameraImageSize.x = 1;
    }

    if (size.y <= 0) {
        this->cameraImageSize.y = 1;
    }
}

void Settings::setCameraImageWidth(int width) {
    this->cameraImageSize.x = width > 0 ? width : 1;
}

void Settings::setCameraImageHeight(int height) {
    this->cameraImageSize.y = height > 0 ? height : 1;
}

bool Settings::isVideoEnabled(void) {
    return this->videoEnabled;
}

void Settings::setVideoEnabled(bool enable) {
    this->videoEnabled = enable;
}
