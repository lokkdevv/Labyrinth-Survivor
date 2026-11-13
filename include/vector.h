#pragma once

#include <raylib.h>
#include <math.h>

float vectorLengthSqr(Vector2 vector);
float vectorLength(Vector2 vector);
Vector2 vectorScale(Vector2 vector, float scale);
Vector2 getDirection(int keyDown, int keyUp, int keyLeft, int keyRight);
Vector2 normalize(Vector2 vector);
