#ifndef UTIL_H
#define UTIL_H

struct Vector2D {
    float x;
    float y;
};

struct TrackResult {
    int id;
    Vector2D pos;
};

void sendClosePacket(int port);

#endif // UTIL_H
