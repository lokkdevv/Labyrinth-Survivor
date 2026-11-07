#include <raylib.h>
#include "utils.h"

#define TARGET_WIDTH 480
#define TARGET_HEIGHT 270

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(TARGET_WIDTH, TARGET_HEIGHT, "Labyrinth Survivor");
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(TARGET_WIDTH, TARGET_HEIGHT);

    float scale;
    Rectangle srcRect;
    Rectangle destRect;

    Vector2 pos = {TARGET_WIDTH/2, TARGET_HEIGHT/2};

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W))
            pos.y--;
        if (IsKeyDown(KEY_S))
            pos.y++;
        if (IsKeyDown(KEY_A))
            pos.x--;
        if (IsKeyDown(KEY_D))
            pos.x++;

        
        scale = minf((float)GetScreenWidth()/TARGET_WIDTH, (float)GetScreenHeight()/TARGET_HEIGHT);
        srcRect = (Rectangle){0, 0, (float)TARGET_WIDTH, (float)-TARGET_HEIGHT};
        destRect = (Rectangle){(float)(GetScreenWidth()-(TARGET_WIDTH*scale))*0.5,
                            (float)(GetScreenHeight()-(TARGET_HEIGHT*scale))*0.5,
                            (float)TARGET_WIDTH*scale,
                            (float)TARGET_HEIGHT*scale
                            };

        BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        DrawCircle(pos.x, pos.y, 10, BROWN);
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
