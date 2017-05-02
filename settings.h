#ifndef SETTINGS_H
#define SETTINGS_H

#include "util.h"

typedef struct {
    int arucoID;
    int robotID;
} ArucoIDPair;

class Settings
{
    static Settings* s_instance;

    Vector2D cameraImageSize;
    bool videoEnabled;
    bool robotColourEnabled;
    bool imageFlip;

    int posHistorySampleInterval;
    bool showAverageRobotPos;

    Settings(void);

public:
    std::vector<ArucoIDPair*> idMapping;

    static Settings* instance() {
        if (s_instance == NULL) {
            s_instance = new Settings();
        }

        return s_instance;
    }

    static void deleteInstance() {
        if (s_instance != NULL) {
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

    bool isRobotColourEnabled(void);
    void setRobotColourEnabled(bool enable);

    int getPosHistorySampleInterval(void);
    void setPosHistorySampleInterval(int interval);

    bool isImageFlipped(void);
    void setImageFlipEnabled(bool enable);

    bool isShowAveragePos(void);
    void setShowAveragePos(bool enable);
};

#endif // SETTINGS_H
