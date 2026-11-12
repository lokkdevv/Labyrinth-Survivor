#pragma once

#include <raylib.h>
#include <math.h>
#include "utils.h"

typedef struct {
    float x;
    float y;
    float radius;
} Circle;

typedef struct {
    Rectangle rec;
    int movable;
} CollisionRec;

typedef struct {
    Circle circle;
    int movable;
} CollisionCircle;

typedef struct {
    int interacted;
    CollisionCircle circle;
    Texture2D texture;
} Object;

void canInteract(CollisionRec interaction_box, Object *object);
void BBcollide(CollisionRec *collision_rectangle_1, 
        CollisionRec *collision_rectangle_2); // Collision between two recs
void BCcollide(CollisionRec collision_rectangle,
        CollisionCircle *collision_circle); // Collision between a rec and a circle
