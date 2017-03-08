#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QTime>
#include <opencv2/core.hpp>

struct Vector2D {
    float x;
    float y;
};

struct TrackResult {
    int id;
    Vector2D pos;
    int angle;
};

struct StateTransition {
    QString oldState;
    QString newState;
    QTime time;
};

void sendClosePacket(int port);

cv::Scalar colourGen(void);

double square(double val);

#endif // UTIL_H
