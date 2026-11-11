#include "../include/object.h"
#include <raylib.h>

static int id = 0;

void interact() {

}

void deleteObject(Object *object) {
    if (object->texture.id != 0)
        UnloadTexture(object->texture);
    free(object);
}

Object* initObject(Vector2 position, float radius, Image image, char* name) {
    Object *object = (Object*)malloc(sizeof(Object));

    object->id = id;
    id++;
    object->circle.circle.x = position.x;
    object->circle.circle.y = position.y;
    object->circle.circle.radius = radius;
    object->name = name;
    Texture2D texture = LoadTextureFromImage(image);
    object->texture = texture;

    return object;
}

void updateObject(Object *object) {
    if (object->interacted)
        interact();
}

void drawObject(Object *object) {
    Vector2 origin = {0, 0};
    Rectangle source = {0, 0,
        object->texture.width, object->texture.height};
    Rectangle dest = {object->circle.circle.x, object->circle.circle.y,
        object->texture.width/4, object->texture.height/4};
    //DrawTexturePro(object->texture, source, dest, origin, 0.0f, WHITE);
    DrawCircle(object->circle.circle.x, object->circle.circle.y, 
            object->circle.circle.radius, debugGreen);
}


