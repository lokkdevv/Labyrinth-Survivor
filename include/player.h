#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "utils.h"
#include "collision.h"

typedef struct {
    CollisionCircle collisionCircle;
    CollisionRec interactionBox;
    Vector2 velocity;
    Vector2 direction;
    float acceleration;
    float friction;
    float max_speed;
    int money;
} Player;

void SetPlayerCollisionShape(Player *player, CollisionCircle collision_circle);
Player* initPlayer(Vector2 initialPosition, float acceleration, float friction, float max_speed,
        float physics_update_interval, float collision_radius);
void deletePlayer(Player* player);
void updateAndMovePlayer(Player *player);
void drawPlayer(const Player *player);
