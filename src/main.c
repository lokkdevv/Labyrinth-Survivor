#include <raylib.h>
#include "../include/utils.h"
#include "../include/player.h"
#include "../include/collision.h"

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

    Vector2 startingPos = (Vector2){TARGET_WIDTH/2, TARGET_HEIGHT/2};
    Player player;
    
    initPlayer(&player, startingPos, ACCELERATION, FRICTION, MAX_SPEED, 1.0f/60.0f);

    CollisionRec player_collision_shape_2 = {(Rectangle){50, 50, 50, 50}, 1};
    CollisionCircle player_collision_shape = {(Circle){50, 50, 30}, 1};
    SetPlayerCollisionShape(&player, CIRCLE_COLLISION, 
            player_collision_shape_2, player_collision_shape);

    CollisionRec wall = {{400, 500, 300, 500}, 0};

    RenderTexture2D target = LoadRenderTexture(TARGET_WIDTH, TARGET_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    float scale;
    Rectangle srcRect;
    Rectangle destRect;

    double delta;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        updatePlayer(&player, delta);
        movePlayer(&player);

        //BBcollide(&player.collisionRec, &wall);
        BCcollide(wall, &player.collisionCircle);

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
        //drawPlayer(&player, DRAW_RECT);
        drawPlayer(&player, DRAW_CIRCLE);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(target.texture, srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        EndDrawing();
    }
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
