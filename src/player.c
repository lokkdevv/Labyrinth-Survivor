#include "player.h"
#include <raylib.h>

static float physicsUpdateInterval;
static float physicsUpdateTimer;
static float delta;

void setPlayerTexture(Player *player, Texture2D text) {
    player->texture = text;
}

void initPlayer(Player *player, Vector2 startPos, float acc, float fric, float max_s) {
    player->velocity = (Vector2){0, 0};
    player->direction = (Vector2){0, 0};
    player->position = startPos;

    player->acceleration = acc;
    player->friction = fric;
    player->max_speed = max_s;
}

void initPhysics(float phInterval) {
    physicsUpdateInterval = phInterval;
    physicsUpdateTimer = 0;
}

void updatePlayer(Player *player, float dlt) {
    delta = dlt;
    physicsUpdateTimer += delta;
    player->direction = getDirection(KEY_S, KEY_W, KEY_A, KEY_D);
    player->direction = normalize(player->direction);
}

void updatePhysics(Player *player) {
    if (physicsUpdateTimer >= physicsUpdateInterval) {
        if (vectorLengthSqr(player->direction) > 0) {
            player->velocity.x += player->direction.x * player->acceleration;
            player->velocity.y += player->direction.y * player->acceleration;
        } else {
            if (player->velocity.x > 0) player->velocity.x = maxf(0, player->velocity.x - player->friction);
            else if (player->velocity.x < 0) player->velocity.x = minf(0, player->velocity.x + player->friction);
            if (player->velocity.y > 0) player->velocity.y = maxf(0, player->velocity.y - player->friction);
            else if (player->velocity.y < 0) player->velocity.y = minf(0, player->velocity.y + player->friction);
        }

        if (vectorLengthSqr(player->velocity) >= (player->max_speed*player->max_speed))
            player->velocity = vectorScale(player->direction, player->max_speed);

        physicsUpdateTimer = 0;
    }
}

void movePlayer(Player *player) {
    player->position.x += player->velocity.x * delta;
    player->position.y += player->velocity.y * delta;
}

void drawPlayer(const Player *player, DRAW_TYPE draw_type) {
    switch (draw_type) {
        case DRAW_CIRCLE:
            DrawCircle(player->position.x, player->position.y, 30, BROWN);
            break;
        case DRAW_TEXTURE:
            DrawTexture(player->texture, player->position.x, player->position.y, WHITE);
            break;
        default:
            break;
    }
}

