#include "../include/object.h"

#define TOWER1_WIDTH 100
#define TOWER1_HEIGHT 190
#define TOWER1_COLOR (Color){200, 66, 100, 255}

#define TOWER2_WIDTH 120
#define TOWER2_HEIGHT 180
#define TOWER2_COLOR (Color){100, 100, 255, 255}

#define RANGE 220
#define MAX_TOWERS 6

#define ARROW_INTERVAL 1.0f
#define LIGHTNING_INTERVAL 2.0f

#define PROJECTILE_SPEED 600.0f
#define LIGHTNING_DURATION 0.2f

#define ARROW_DAMAGE 40
#define LIGHTNING_DAMAGE 100

#define ARROW_COLOR BROWN
#define LIGHTNING_COLOR BLUE

int draw_ui = 0;
static int hovering = 0;

static int price[2] = {50, 130};

static Tower towers[MAX_TOWERS];
static int tower_count = 0;

static TowerRuntime tower_runtime[MAX_TOWERS] = {0};

void deleteObject(Object *object) {
    free(object);
}

Object *initObject(Vector2 position, float radius) {
    Object *object = malloc(sizeof(Object));

    if (object == NULL)
        return NULL;

    object->circle.circle.x = position.x;
    object->circle.circle.y = position.y;
    object->circle.circle.radius = radius;

    object->type = 2;
    object->interacted = 0;

    return object;
}

void updateObject(Object *object, Player *player) {
    if (object == NULL || player == NULL)
        return;

    if (!object->interacted || object->type != 2)
        return;

    draw_ui = 1;

    if (IsKeyPressed(KEY_E) && player->money >= price[hovering]) {
        if (build(object, hovering))
            player->money -= price[hovering];
    }
}

void updateUI(void) {
    if (draw_ui && IsKeyPressed(KEY_TAB))
        hovering = (hovering + 1) % 2;
}

int build(Object *object, int selected) {
    if (object == NULL || object->type != 2)
        return 0;

    if (selected < 0 || selected > 1 || tower_count >= MAX_TOWERS)
        return 0;

    Vector2 pos = {
        object->circle.circle.x,
        object->circle.circle.y
    };

    towers[tower_count] = (Tower){selected, pos};
    tower_runtime[tower_count] = (TowerRuntime){0};

    tower_count++;
    object->type = selected;

    return 1;
}

void updateTowers(Enemy *enemies[], int enemy_count) {
    float delta = GetFrameTime();

    for (int t = 0; t < tower_count; ++t) {
        TowerRuntime *runtime = &tower_runtime[t];

        runtime->cooldown -= delta;

        if (runtime->cooldown < 0)
            runtime->cooldown = 0;

        if (runtime->effect_timer > 0) {
            runtime->effect_timer -= delta;

            if (runtime->effect_timer < 0)
                runtime->effect_timer = 0;

            int target = runtime->effect_target;

            if (target >= 0 && target < enemy_count &&
                enemies[target] != NULL) {
                runtime->effect_pos = (Vector2){
                    enemies[target]->circle.circle.x,
                    enemies[target]->circle.circle.y
                };
            }
        }

        if (runtime->projectile_active) {
            int target = runtime->target_index;

            if (target < 0 || target >= enemy_count || enemies[target] == NULL || enemies[target]->health <= 0) {
                runtime->projectile_active = 0;
            } else {
                Enemy *enemy = enemies[target];

                Vector2 enemy_pos = {enemy->circle.circle.x, enemy->circle.circle.y};

                runtime->projectile_pos = Vector2MoveTowards(runtime->projectile_pos,
                    enemy_pos, PROJECTILE_SPEED * delta);

                if (Vector2DistanceSqr(runtime->projectile_pos, enemy_pos) <= 0.01f) {
                    enemy->health -= ARROW_DAMAGE;
                    runtime->projectile_active = 0;
                }
            }
        }

        if (runtime->cooldown > 0 || (towers[t].tower_type == 0 && runtime->projectile_active))
            continue;

        int closest_enemy = -1;
        float closest_distance = RANGE * RANGE;

        Vector2 tower_pos = towers[t].position;

        for (int e = 0; e < enemy_count; ++e) {
            if (enemies[e] == NULL || enemies[e]->health <= 0)
                continue;

            Vector2 enemy_pos = {enemies[e]->circle.circle.x, enemies[e]->circle.circle.y};

            float distance = Vector2DistanceSqr(tower_pos, enemy_pos);

            if (distance <= closest_distance) {
                closest_distance = distance;
                closest_enemy = e;
            }
        }

        if (closest_enemy == -1)
            continue;

        Enemy *enemy = enemies[closest_enemy];

        if (towers[t].tower_type == 0) {
            runtime->projectile_active = 1;
            runtime->target_index = closest_enemy;

            runtime->projectile_pos = (Vector2){
                tower_pos.x,
                tower_pos.y - TOWER1_HEIGHT / 2.0f
            };

            runtime->cooldown = ARROW_INTERVAL;

        } else if (towers[t].tower_type == 1) {
            enemy->health -= LIGHTNING_DAMAGE;

            runtime->effect_timer = LIGHTNING_DURATION;
            runtime->effect_target = closest_enemy;

            runtime->effect_pos = (Vector2){
                enemy->circle.circle.x,
                enemy->circle.circle.y
            };

            runtime->cooldown = LIGHTNING_INTERVAL;
        }
    }
}

void resetTowers(void) {
    tower_count = 0;

    for (int x = 0; x < MAX_TOWERS; ++x) {
        towers[x] = (Tower){0};
        tower_runtime[x] = (TowerRuntime){0};
    }

    draw_ui = 0;
    hovering = 0;
}

void drawObject(Object *object) {
    if (object == NULL)
        return;

    //DrawCircle(object->circle.circle.x, object->circle.circle.y, RANGE, debugRed);
    DrawCircle(object->circle.circle.x, object->circle.circle.y, object->circle.circle.radius, debugGreen);
}

void drawTowers(void) {
    for (int x = 0; x < tower_count; ++x) {
        if (towers[x].tower_type == 0) {
            DrawRectangle(
                towers[x].position.x - TOWER1_WIDTH / 2,
                towers[x].position.y - TOWER1_HEIGHT,
                TOWER1_WIDTH,
                TOWER1_HEIGHT,
                TOWER1_COLOR
            );
        } else {
            DrawRectangle(
                towers[x].position.x - TOWER2_WIDTH / 2,
                towers[x].position.y - TOWER2_HEIGHT,
                TOWER2_WIDTH,
                TOWER2_HEIGHT,
                TOWER2_COLOR
            );
        }

        if (tower_runtime[x].projectile_active) {
            Vector2 pos = tower_runtime[x].projectile_pos;
            DrawCircle(pos.x, pos.y, 5, ARROW_COLOR);
        }

        if (tower_runtime[x].effect_timer > 0) {
            Vector2 pos = tower_runtime[x].effect_pos;
            DrawRectangle(pos.x - 15, pos.y - 15, 30, 30, LIGHTNING_COLOR);
        }
    }
}

void drawBuildUI(void) {
    if (!draw_ui)
        return;

    DrawRectangle(0, 1080 - 200, 1920, 200, WHITE);

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
    } else {
        DrawRectangle(margin, boxY, boxWidth, boxHeight, GRAY);
        DrawRectangle(margin + boxWidth + gap, boxYHover, boxWidth, boxHeightHover, GRAY);
    }

    DrawText("Arrow Tower", margin + 300, boxY + 50, 40, RAYWHITE);
    DrawText("Lightning Tower", margin + boxWidth + 340, boxY + 50, 40, RAYWHITE);
    DrawText("Price: 50", margin + 300, boxY + 100, 30, YELLOW);
    DrawText("Price: 130", margin + boxWidth + 340, boxY + 100, 30, YELLOW);
}
