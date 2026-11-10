#pragma once

#include <raylib.h>
#include "vector.h"
#include "utils.h"

typedef enum {
    DRAW_TEXTURE = 0,
    DRAW_CIRCLE,
    DRAW_RECT
} DRAW_TYPE;

typedef struct {
    Texture2D texture;
    Vector2 velocity;
    Vector2 direction;
    Vector2 position;
    float acceleration;
    float friction;
    float max_speed;
} Player;

void setPlayerTexture(Player *player, Texture2D texture);
void initPlayer(Player *player, Vector2 initialPosition, float acceleration, float friction, float max_speed);
void initPhysics(float physics_update_interval);
void updatePlayer(Player *player, float delta);
void updatePhysics(Player *player);
void movePlayer(Player *player);
void drawPlayer(const Player *player, DRAW_TYPE draw_type);
