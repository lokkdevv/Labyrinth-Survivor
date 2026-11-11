#include "../include/player.h"

static float physicsUpdateInterval;
static float physicsUpdateTimer;
static float delta;

void setPlayerTexture(Player *player, Texture2D text) {
    player->texture = text;
}

void SetPlayerCollisionShape(Player *player, COLLISION_SHAPE collision_shape, 
        CollisionRec collisionRec, CollisionCircle collisionCircle) {
    switch (collision_shape) {
        case RECTANGLE_COLLISION:
            player->collisionRec = collisionRec;
            break;
        case CIRCLE_COLLISION:
            player->collisionCircle = collisionCircle;
            break;
        default:
            break;
    }
}

void initPlayer(Player *player, Vector2 startPos, float acc, float fric, float max_s, float phInterval) {
    player->velocity = (Vector2){0, 0};
    player->direction = (Vector2){0, 0};
    player->collisionRec.rec.x = startPos.x;
    player->collisionRec.rec.y = startPos.y;

    player->acceleration = acc;
    player->friction = fric;
    player->max_speed = max_s;

    physicsUpdateInterval = phInterval;
    physicsUpdateTimer = 0;
}

void updatePlayer(Player *player, float dlt) {
    delta = dlt;
    physicsUpdateTimer += delta;
    player->direction = getDirection(KEY_S, KEY_W, KEY_A, KEY_D);
    player->direction = normalize(player->direction);

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
    player->collisionRec.rec.x += player->velocity.x * delta;
    player->collisionRec.rec.y += player->velocity.y * delta;

    player->collisionCircle.circle.x += player->velocity.x * delta;
    player->collisionCircle.circle.y += player->velocity.y * delta;
}

void drawPlayer(const Player *player, DRAW_TYPE draw_type) {
    switch (draw_type) {
        case DRAW_CIRCLE:
            DrawCircle(player->collisionCircle.circle.x, player->collisionCircle.circle.y,
                    player->collisionCircle.circle.radius, BROWN);
            break;
        case DRAW_TEXTURE:
            DrawTexture(player->texture, player->collisionCircle.circle.x - (player->texture.width)/2,
                    player->collisionCircle.circle.y - (player->texture.height)/2, WHITE);
            break;
        case DRAW_RECT:
            DrawRectangle(player->collisionRec.rec.x, player->collisionRec.rec.y,
                    player->collisionRec.rec.width, player->collisionRec.rec.height, BROWN);
        default:
            break;
    }
}

