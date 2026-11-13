#include "../include/vector.h"

inline float vectorLengthSqr(Vector2 vector) {
    return vector.x*vector.x+vector.y*vector.y;
}

inline float vectorLength(Vector2 vector) {
    return sqrt(vector.x*vector.x+vector.y*vector.y);
}

inline Vector2 vectorScale(Vector2 vector, float scale) {
    return (Vector2){vector.x * scale, vector.y * scale};
}

Vector2 getDirection(int keyDown, int keyUp, int keyLeft, int keyRight) {
    return (Vector2){
            (float)((int)IsKeyDown(keyRight)-(int)IsKeyDown(keyLeft)),
            (float)((int)IsKeyDown(keyDown)-(int)IsKeyDown(keyUp))
    };
}

Vector2 normalize(Vector2 vector) {
    float amount = (float)(vector.x*vector.x)+(vector.y*vector.y);
    if (amount < 0.0001f)
        return (Vector2){0, 0};
    float length = sqrt(amount);
    return (Vector2){vector.x/length, vector.y/length};
}
