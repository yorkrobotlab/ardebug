#ifndef SETTINGS_H
#define SETTINGS_H

#include "util.h"

class Settings
{
    static Settings* s_instance;

    Vector2D cameraImageSize;
    bool videoEnabled;

    Settings(void);

public:
    static Settings* instance() {
        if (!s_instance) {
            s_instance = new Settings;
        }

        return s_instance;
    }

    static void deleteInstance() {
        if (s_instance) {
            delete s_instance;
            s_instance = 0;
        }
    }

    Vector2D getCameraImageSize(void);
    void setCameraImageSize(Vector2D size);
    void setCameraImageWidth(int width);
    void setCameraImageHeight(int height);

    bool isVideoEnabled(void);
    void setVideoEnabled(bool enable);
};

#endif // SETTINGS_H
