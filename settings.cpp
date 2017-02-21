#include "settings.h"

Settings::Settings(void) {
    cameraImageSize.x = 2096;
    cameraImageSize.y = 1180;
}

Vector2D Settings::getCameraImageSize(void) {
    return cameraImageSize;
}

void Settings::setCameraImageSize(Vector2D size) {
    cameraImageSize = size;

    if (size.x <= 0) {
        size.x = 1;
    }

    if (size.y <= 0) {
        size.y = 1;
    }
}

void Settings::setCameraImageWidth(int width) {
    cameraImageSize.x = width > 0 ? width : 1;
}

void Settings::setCameraImageHeight(int height) {
    cameraImageSize.y = height > 0 ? height : 1;
}
