#pragma once

#include <raylib.h>
#include <stdlib.h>
#include "vector.h"
#include "utils.h"
#include "collision.h"

typedef enum {
    DRAW_TEXTURE = 0,
    DRAW_CIRCLE,
} DRAW_TYPE;


typedef struct {
    CollisionCircle collisionCircle;
    CollisionRec interactionBox;
    Texture2D texture;
    Vector2 velocity;
    Vector2 direction;
    float acceleration;
    float friction;
    float max_speed;
} Player;

void setPlayerTexture(Player *player, Image image);
void SetPlayerCollisionShape(Player *player, CollisionCircle collision_circle);
Player* initPlayer(Vector2 initialPosition, float acceleration, float friction, float max_speed,
        float physics_update_interval, float collision_radius);
void deletePlayer(Player* player);
void updateAndMovePlayer(Player *player);
void drawPlayer(const Player *player, DRAW_TYPE draw_type);
