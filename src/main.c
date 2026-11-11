#include <raylib.h>
#include <stdio.h>
#include "../include/utils.h"
#include "../include/collision.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/object.h"

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

#define ACCELERATION 40.0f
#define FRICTION 45.0f
#define MAX_SPEED 450.0f

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(TARGET_WIDTH, TARGET_HEIGHT, "Labyrinth Survivor");
    SetTargetFPS(90);

    // Player setup //
    Vector2 startingPos = (Vector2){TARGET_WIDTH/2, TARGET_HEIGHT/2};
    
    Player *player = initPlayer(startingPos, ACCELERATION, FRICTION, MAX_SPEED, 1.0f/60.0f, 30);
    ///////////////////
    /// Enemy setup ///
    Image image = LoadImage("resources/sprites/death_0.png");
    Enemy *enemy = initEnemy(image, 20, 200, 30, (Vector2){30, 30}, 30);
    ///////////////////
    /// Object setup //
    Object *object = initObject((Vector2){600, 300}, 20, image, "sword");
    ///////////////////

    CollisionRec wall = {{400, 500, 300, 500}, 0}; // Collision wall //

    // BoilerPlate code //
    RenderTexture2D target = LoadRenderTexture(TARGET_WIDTH, TARGET_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    float scale;
    Rectangle srcRect;
    Rectangle destRect;

    double delta;
    //////////////////////

    while (!WindowShouldClose()) {
        updateAndMovePlayer(player);
        Vector2 player_pos = {player->collisionCircle.circle.x, player->collisionCircle.circle.y};
        updateAndMoveEnemy(enemy, player_pos);
        updateObject(object);

        canInteract(player->interactionBox, object);
        BCcollide(wall, &player->collisionCircle);

        if (object->interacted == 1)
            printf("Interacted with object with id: %d\n", object->id);

        ////// BoilerPlate //////
        scale = minf((float)GetScreenWidth()/TARGET_WIDTH, (float)GetScreenHeight()/TARGET_HEIGHT);
        srcRect = (Rectangle){0, 0, (float)TARGET_WIDTH, (float)-TARGET_HEIGHT};
        destRect = (Rectangle){(float)(GetScreenWidth()-(TARGET_WIDTH*scale))*0.5,
                            (float)(GetScreenHeight()-(TARGET_HEIGHT*scale))*0.5,
                            (float)TARGET_WIDTH*scale,
                            (float)TARGET_HEIGHT*scale
                            };
        /////////////////////////
        
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        DrawRectangleRec(wall.rec, BLACK);
        drawPlayer(player, DRAW_CIRCLE);
        drawEnemy(enemy);
        drawObject(object);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(target.texture, srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        EndDrawing();
    }
    UnloadRenderTexture(target);
    deleteEnemy(enemy);
    deletePlayer(player);
    deleteObject(object);
    CloseWindow();
    return 0;
}
