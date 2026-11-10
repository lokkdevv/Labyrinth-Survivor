#include "player.h"

float physicsUpdateInterval = 1.0f / 60.0f;
float physicsUpdateTimer = 0;
double delta;

Vector2 velocity = {0, 0};
Vector2 direction = {0, 0};
Vector2 position = {TARGET_WIDTH/2, TARGET_HEIGHT/2};

float acceleration = 35.0f;
float friction = 40.0f;
float max_speed = 230.0f;

physicsUpdateTimer += delta;

direction = getDirection(KEY_S, KEY_W, KEY_A, KEY_D);
direction = normalize(direction);

////// Physics Update //////
if (physicsUpdateTimer >= physicsUpdateInterval) {
    if (vectorLengthSqr(direction) > 0) {
        velocity.x += direction.x * acceleration;
        velocity.y += direction.y * acceleration;
    } else {
        if (velocity.x > 0) velocity.x = maxf(0, velocity.x - friction);
        else if (velocity.x < 0) velocity.x = minf(0, velocity.x + friction);
        if (velocity.y > 0) velocity.y = maxf(0, velocity.y - friction);
        else if (velocity.y < 0) velocity.y = minf(0, velocity.y + friction);
    }

    if (vectorLengthSqr(velocity) >= (max_speed*max_speed))
        velocity = vectorScale(direction, max_speed);

    physicsUpdateTimer = 0;
}
////////////////////////////

position.x += velocity.x * delta;
position.y += velocity.y * delta;

DrawCircle(position.x, position.y, 30, BROWN);
EndTextureMode();
