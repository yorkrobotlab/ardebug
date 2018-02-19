#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QTime>
#include <opencv2/core.hpp>

struct Vector2D {
    double x;
    double y;
};

struct Pose
{
    Vector2D position;
    double orientation;
};

struct TrackResult {
    int id;
    Pose pose;
};

struct StateTransition {
    QString oldState;
    QString newState;
    QTime time;
};

void sendClosePacket(int port);

double square(double val);

#endif // UTIL_H
