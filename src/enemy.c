#include "../include/enemy.h"
#include <raylib.h>

static float delta;

Enemy* initEnemy(float attack_spd, float move_spd, float damage, Vector2 starting_pos, float radius) {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    enemy->circle.circle.radius = radius;
    enemy->damage = damage;
    enemy->attack_speed = attack_spd;
    enemy->move_speed = move_spd;
    enemy->circle.circle.x = starting_pos.x;
    enemy->circle.circle.y = starting_pos.y;
    return enemy;
}

void deleteEnemy(Enemy *enemy) {
    free(enemy);
}

void updateAndMoveEnemy(Enemy *enemy, Vector2 target) {
    Vector2 direction;
    delta = GetFrameTime();
    direction.x = target.x - enemy->circle.circle.x ;
    direction.y = target.y - enemy->circle.circle.y ;
    direction = normalize(direction);
    enemy->direction = direction;
    enemy->circle.circle.x += enemy->direction.x * enemy->move_speed * delta;
    enemy->circle.circle.y += enemy->direction.y * enemy->move_speed * delta;
}

void drawEnemy(Enemy *enemy) {
    DrawCircle(enemy->circle.circle.x, enemy->circle.circle.y, enemy->circle.circle.radius, debugRed);
}


