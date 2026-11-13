#include "../include/enemy.h"

static float delta;

Enemy* initEnemy(float attack_spd, float move_spd, float damage, Vector2 starting_pos, float radius) {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    enemy->circle.circle.radius = radius;
    enemy->damage = damage;
    enemy->health = 200;
    enemy->attack_speed = attack_spd;
    enemy->move_speed = move_spd;
    enemy->circle.circle.x = starting_pos.x;
    enemy->circle.circle.y = starting_pos.y;
    enemy->current_target = 0;
    enemy->reached_end = 0;
    return enemy;
}

void deleteEnemy(Enemy *enemy) {
    free(enemy);
}

void updateAndMoveEnemy(Enemy *enemy, Vector2 targets[]) {
    float delta = GetFrameTime();

    if (enemy->health <= 0 || enemy->current_target >= 7)
        return;

    Vector2 enemy_pos = {enemy->circle.circle.x, enemy->circle.circle.y};

    Vector2 target = targets[enemy->current_target];

    if (vectorLengthSqr((Vector2){target.x - enemy_pos.x, target.y - enemy_pos.y}) <= 100) {
        enemy->current_target++;
        if (enemy->current_target >= 7) {
            enemy->reached_end = 1;
            return;
        }
        target = targets[enemy->current_target];
    }
    enemy_pos = Vector2MoveTowards(enemy_pos, target, enemy->move_speed * delta);

    enemy->circle.circle.x = enemy_pos.x;
    enemy->circle.circle.y = enemy_pos.y;
}

void drawEnemy(Enemy *enemy) {
    DrawCircle(enemy->circle.circle.x, enemy->circle.circle.y, enemy->circle.circle.radius, debugRed);
}


