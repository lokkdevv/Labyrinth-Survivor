#include "../include/player.h"

#define LAUNCH_FORCE 300

static float physicsUpdateInterval;
static float physicsUpdateTimer;
static float delta;

static Vector2 lastNonNegativeDir;

static int launch_force = 0;

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

    player->money = 200;

    physicsUpdateInterval = phInterval;
    physicsUpdateTimer = 0;

    lastNonNegativeDir = (Vector2){0, 0};
    player->interactionBox.rec.width = 100;
    player->interactionBox.rec.height = 70;

    return player;
}

void deletePlayer(Player *player) {
    free(player);
}

void updateAndMovePlayer(Player *player) {
    delta = GetFrameTime();
    physicsUpdateTimer += delta;
    player->direction = getDirection(KEY_S, KEY_W, KEY_A, KEY_D);
    player->direction = normalize(player->direction);

    if (player->collisionCircle.circle.x < 0) {
        player->direction = (Vector2){1, 0};
        launch_force = LAUNCH_FORCE;
    }
    if (player->collisionCircle.circle.x > 1920) {
        player->direction = (Vector2){-1, 0};
        launch_force = LAUNCH_FORCE;
    }
    if (player->collisionCircle.circle.y > 1080) {
        player->direction= (Vector2){0, -1};
        launch_force = LAUNCH_FORCE;
    }
    if (player->collisionCircle.circle.y < 0) {
        player->direction = (Vector2){0, 1};
        launch_force = LAUNCH_FORCE;
    }

    if (physicsUpdateTimer >= physicsUpdateInterval) {
        if (vectorLengthSqr(player->direction) > 0) {
            player->velocity.x += player->direction.x * (launch_force > 0 ? launch_force : player->acceleration);
            player->velocity.y += player->direction.y * (launch_force > 0 ? launch_force : player->acceleration);
            launch_force = 0;
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

    player->interactionBox.rec.x = player->collisionCircle.circle.x - player->interactionBox.rec.width/2 +
        lastNonNegativeDir.x * player->interactionBox.rec.width/2;
    player->interactionBox.rec.y = player->collisionCircle.circle.y - player->interactionBox.rec.height/2+
        lastNonNegativeDir.y * player->interactionBox.rec.height/2;
}

void drawPlayer(const Player *player) {
    DrawCircle(player->collisionCircle.circle.x, player->collisionCircle.circle.y,
            player->collisionCircle.circle.radius, debugGreen);
    DrawRectangle(player->interactionBox.rec.x,
            player->interactionBox.rec.y,
            player->interactionBox.rec.width, player->interactionBox.rec.height, debugYellow);

    DrawText("Money:", 1920/2-85, 5, 30, YELLOW);
    char money[40];
    snprintf(money, sizeof(money), "%d", player->money);
    DrawText(money, 1920/2-50, 30, 30, YELLOW);
}

