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

extern int draw_ui;

Object* initObject(Vector2 starting_position, float collision_radius);
void updateUI();
int build(Object *object, int selected);
void deleteObject(Object *object);
void updateObject(Object *object, Player *player, Enemy *enemy);
void drawObject(Object *object); // 1 to draw ui 0 to not draw ui
