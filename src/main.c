#include <raylib.h>
#include "utils.h"
#include "player.h"

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

#define ACCELERATION 40.0f
#define FRICTION 45.0f
#define MAX_SPEED 250.0f

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(TARGET_WIDTH, TARGET_HEIGHT, "Labyrinth Survivor");
    SetTargetFPS(90);

    Player player;
    initPlayer(&player, (Vector2){TARGET_WIDTH/2, TARGET_HEIGHT/2}, ACCELERATION, FRICTION, MAX_SPEED);
    initPhysics((float)1.0f/120.0f);

    RenderTexture2D target = LoadRenderTexture(TARGET_WIDTH, TARGET_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    float scale;
    Rectangle srcRect;
    Rectangle destRect;

    double delta;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        updatePlayer(&player, delta);
        updatePhysics(&player);
        movePlayer(&player);

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
