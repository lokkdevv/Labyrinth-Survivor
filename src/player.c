#include "../include/player.h"

static float physicsUpdateInterval;
static float physicsUpdateTimer;
static float delta;

static CollisionRec interactionBox;
static Vector2 lastNonNegativeDir;

void setPlayerTexture(Player *player, Texture2D text) {
    player->texture = text;
}

Player* initPlayer(Vector2 startPos, float acc, float fric, float max_s, float phInterval, float radius) {
    Player *player = (Player*)malloc(sizeof(Player));
    player->velocity = (Vector2){0, 0};
    player->direction = (Vector2){0, 0};
    player->collisionCircle.circle.x = startPos.x;
    player->collisionCircle.circle.y = startPos.y;
    player->collisionCircle.circle.radius = radius;

    player->acceleration = acc;
    player->friction = fric;
    player->max_speed = max_s;

    physicsUpdateInterval = phInterval;
    physicsUpdateTimer = 0;

    lastNonNegativeDir = (Vector2){0, 0};
    interactionBox.rec.width = 100;
    interactionBox.rec.height = 70;

    return player;
}

void deletePlayer(Player *player) {
    if (!player->texture.id)
        UnloadTexture(player->texture);
    free(player);
}

void updateAndMovePlayer(Player *player) {
    delta = GetFrameTime();
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

    player->collisionCircle.circle.x += player->velocity.x * delta;
    player->collisionCircle.circle.y += player->velocity.y * delta;

    if (vectorLengthSqr(player->direction)>0.0001f)
        lastNonNegativeDir = player->direction;

    interactionBox.rec.x = player->collisionCircle.circle.x + lastNonNegativeDir.x * interactionBox.rec.width/2;
    interactionBox.rec.y = player->collisionCircle.circle.y + lastNonNegativeDir.y * interactionBox.rec.height/2;
}

void drawPlayer(const Player *player, DRAW_TYPE draw_type) {
    switch (draw_type) {
        case DRAW_CIRCLE:
            DrawCircle(player->collisionCircle.circle.x, player->collisionCircle.circle.y,
                    player->collisionCircle.circle.radius, debugGreen);
            break;
        case DRAW_TEXTURE:
            DrawTexture(player->texture, player->collisionCircle.circle.x - (player->texture.width)/2,
                    player->collisionCircle.circle.y - (player->texture.height)/2, WHITE);
            break;
        default:
            break;
    }
    DrawRectangle(interactionBox.rec.x-interactionBox.rec.width/2, interactionBox.rec.y-interactionBox.rec.height/2,
            interactionBox.rec.width, interactionBox.rec.height, debugYellow);
}

