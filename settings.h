#ifndef SETTINGS_H
#define SETTINGS_H

#include "util.h"

class Settings
{
    static Settings* s_instance;

    Vector2D cameraImageSize;

    Settings(void);

public:
    static Settings* instance() {
        if (!s_instance) {
            s_instance = new Settings;
        }

        return s_instance;
    }

    Vector2D getCameraImageSize(void);
    void setCameraImageSize(Vector2D size);
    void setCameraImageWidth(int width);
    void setCameraImageHeight(int height);
};

#endif // SETTINGS_H
