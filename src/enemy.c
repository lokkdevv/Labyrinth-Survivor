#include "../include/enemy.h"
#include <raylib.h>

static float delta;

Enemy* initEnemy(Image image, float attack_spd, float move_spd, float damage, Vector2 starting_pos, float radius) {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    enemy->circle.circle.radius = radius;
    enemy->damage = damage;
    enemy->attack_speed = attack_spd;
    enemy->move_speed = move_spd;
    enemy->circle.circle.x = starting_pos.x;
    enemy->circle.circle.y = starting_pos.y;
    Texture2D texture = LoadTextureFromImage(image);
    enemy->texture = texture;
    return enemy;
}

void deleteEnemy(Enemy *enemy) {
    UnloadTexture(enemy->texture);
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
    Vector2 origin = {0, 0};
    Rectangle source = {0, 0,
        enemy->texture.width, enemy->texture.height};
    Rectangle dest = {enemy->circle.circle.x, enemy->circle.circle.y,
        enemy->texture.width/4, enemy->texture.height/4};
    //DrawTexturePro(enemy->texture, source, dest, origin, 0.0f, WHITE);
    DrawCircle(enemy->circle.circle.x, enemy->circle.circle.y, enemy->circle.circle.radius, debugRed);
}


