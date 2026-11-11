#pragma once

#include <raylib.h>
#include "vector.h"
#include "utils.h"
#include "collision.h"

typedef enum {
    DRAW_TEXTURE = 0,
    DRAW_CIRCLE,
    DRAW_RECT
} DRAW_TYPE;

typedef enum {
    RECTANGLE_COLLISION = 0,
    CIRCLE_COLLISION
} COLLISION_SHAPE;

typedef struct {
    CollisionCircle collisionCircle;
    CollisionRec collisionRec;
    Texture2D texture;
    Vector2 velocity;
    Vector2 direction;
    float acceleration;
    float friction;
    float max_speed;
} Player;

void setPlayerTexture(Player *player, Texture2D texture);
void SetPlayerCollisionShape(Player *player, COLLISION_SHAPE collision_shape, 
        CollisionRec collision_rectangle, CollisionCircle collision_circle);
void initPlayer(Player *player, Vector2 initialPosition, float acceleration, float friction, float max_speed,
        float physics_update_interval);
void updatePlayer(Player *player, float delta);
void movePlayer(Player *player);
void drawPlayer(const Player *player, DRAW_TYPE draw_type);
