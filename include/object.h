#pragma once

#include <raylib.h>
#include <stdlib.h>
#include "enemy.h"
#include "player.h"
#include "collision.h"
#include "utils.h"

typedef struct {
    int tower_type;
    Vector2 position;
} Tower;

typedef struct {
    float cooldown;

    int projectile_active;
    int target_index;
    Vector2 projectile_pos;

    float effect_timer;
    int effect_target;
    Vector2 effect_pos;
} TowerRuntime;

extern int draw_ui;

Object* initObject(Vector2 starting_position, float collision_radius);
void updateUI();
int build(Object *object, int selected);
void attack(int tower_type, Enemy *enemy);
void deleteObject(Object *object);
void updateObject(Object *object, Player *player);
void updateTowers(Enemy *enemies[], int enemy_count);
void resetTowers();
void drawTowers();
void drawBuildUI();
void drawObject(Object *object); // 1 to draw ui 0 to not draw ui
