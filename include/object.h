#pragma once

#include <raylib.h>
#include <stdlib.h>
#include "collision.h"
#include "utils.h"

void interact();
Object* initObject(Vector2 starting_position, float collision_radius, Image image, char* object_name);
void deleteObject(Object *object);
void updateObject(Object *object);
void drawObject(Object *object);
