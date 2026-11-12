#pragma once

#include <raylib.h>
#include <stdlib.h>
#include "collision.h"
#include "vector.h"
#include "utils.h"

typedef struct {
    CollisionCircle circle;
    float move_speed;
    float damage;
    float attack_speed;
    Vector2 direction;
    Vector2 target_pos;
} Enemy;

Enemy* initEnemy(float attack_speed, float move_speed, 
        float damage, Vector2 starting_position, float radius);
void updateAndMoveEnemy(Enemy *enemy, Vector2 target_position);
void deleteEnemy(Enemy *enemy);
void drawEnemy(Enemy *enemy);
