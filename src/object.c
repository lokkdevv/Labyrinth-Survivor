#include "../include/object.h"
#include <raylib.h>

#define TOWER1_WIDTH 100
#define TOWER1_HEIGHT 200
#define TOWER1_COLOR (Color){100, 200, 50, 255}
#define TOWER2_WIDTH 120
#define TOWER2_HEIGHT 209
#define TOWER2_COLOR (Color){255, 200, 50, 255}

int draw_ui = 0;
static int hovering = 0;

static int price[2] = {50, 130};

static Tower towers[6];
static int i = 0;

void deleteObject(Object *object) {
    if (object->texture.id != 0)
        UnloadTexture(object->texture);
    free(object);
}

Object* initObject(Vector2 position, float radius) {
    Object *object = (Object*)malloc(sizeof(Object));

    object->circle.circle.x = position.x;
    object->circle.circle.y = position.y;
    object->circle.circle.radius = radius;

    return object;
}

void updateObject(Object *object, Player *player) {
    if (object->interacted) {
        draw_ui = 1;

        if (IsKeyPressed(KEY_E) && player->money >= price[hovering]) {
            if (i < 6 && build(object, hovering)) {
                player->money -= price[hovering];
            }
        }
    }
}

void updateUI() {
    if (draw_ui && IsKeyPressed(KEY_TAB))
        hovering = (hovering + 1) % 2;
}

int build(Object *object, int select) {
    Vector2 pos = {object->circle.circle.x, object->circle.circle.y};
    for (int x = 0; x<i; ++x) {
        if (pos.x == towers[x].position.x && pos.y == towers[x].position.y)
            return 0;
    }
    towers[i] = (Tower){select, pos};
    i++;
    return 1;
}

void drawObject(Object *object) {
    DrawCircle(object->circle.circle.x, object->circle.circle.y, 
            object->circle.circle.radius, debugGreen);

    for (int x = 0; x<i; ++x) {
        if (towers[x].tower_type == 0) {
            DrawRectangle(towers[x].position.x - TOWER1_WIDTH/2, towers[x].position.y - TOWER1_HEIGHT,
                    TOWER1_WIDTH, TOWER1_HEIGHT, TOWER1_COLOR);
        } else {
            DrawRectangle(towers[x].position.x - TOWER2_WIDTH/2, towers[x].position.y - TOWER2_HEIGHT,
                    TOWER2_WIDTH, TOWER2_HEIGHT, TOWER2_COLOR);
        }
    }


    if (draw_ui) {
        DrawRectangle(0, 1080-200, 1920, 200, WHITE);

        int margin = 20;
        int gap = 20;
        int boxWidth = (1920 - margin * 2 - gap) / 2;
        int boxHeight = 160;
        int boxHeightHover = 180;
        int boxY = 1080 - 200 + 20;
        int boxYHover = 1080 - 200 + 10;

        if (hovering == 0) {
            DrawRectangle(margin, boxYHover, boxWidth, boxHeightHover, GRAY);
            DrawRectangle(margin + boxWidth + gap, boxY, boxWidth, boxHeight, GRAY);
            DrawText("Arrow Tower", margin + 300, boxY+50, 40, RAYWHITE);
            DrawText("Lightning Tower", margin + boxWidth + 340, boxY+50, 40, RAYWHITE);
            DrawText("Price: 50", margin + 300, boxY+100, 30, YELLOW);
            DrawText("Price: 130", margin + boxWidth + 340, boxY+100, 30, YELLOW);
        }
        if (hovering == 1) {
            DrawRectangle(margin, boxY, boxWidth, boxHeight, GRAY);
            DrawRectangle(margin + boxWidth + gap, boxYHover, boxWidth, boxHeightHover, GRAY);
            DrawText("Arrow Tower", margin + 300, boxY+50, 40, RAYWHITE);
            DrawText("Lightning Tower", margin + boxWidth + 340, boxY+50, 40, RAYWHITE);
            DrawText("Price: 50", margin + 300, boxY+100, 30, YELLOW);
            DrawText("Price: 130", margin + boxWidth + 340, boxY+100, 30, YELLOW);
        }
    }
}


