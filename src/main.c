#include <raylib.h>
#include "utils.h"
#include "vector.h"

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(TARGET_WIDTH, TARGET_HEIGHT, "Labyrinth Survivor");
    SetTargetFPS(90);

    RenderTexture2D target = LoadRenderTexture(TARGET_WIDTH, TARGET_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    float scale;
    Rectangle srcRect;
    Rectangle destRect;

    float physicsUpdateInterval = 1.0f / 60.0f;
    float physicsUpdateTimer = 0;
    double delta;

    Vector2 velocity = {0, 0};
    Vector2 direction = {0, 0};
    Vector2 position = {TARGET_WIDTH/2, TARGET_HEIGHT/2};

    float acceleration = 35.0f;
    float friction = 40.0f;
    float max_speed = 230.0f;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();
        physicsUpdateTimer += delta;

        direction = getDirection(KEY_S, KEY_W, KEY_A, KEY_D);
        direction = normalize(direction);

        ////// Physics Update //////
        if (physicsUpdateTimer >= physicsUpdateInterval) {
            if (vectorLengthSqr(direction) > 0) {
                velocity.x += direction.x * acceleration;
                velocity.y += direction.y * acceleration;
            } else {
                if (velocity.x > 0) velocity.x = maxf(0, velocity.x - friction);
                else if (velocity.x < 0) velocity.x = minf(0, velocity.x + friction);
                if (velocity.y > 0) velocity.y = maxf(0, velocity.y - friction);
                else if (velocity.y < 0) velocity.y = minf(0, velocity.y + friction);
            }

            if (vectorLengthSqr(velocity) >= (max_speed*max_speed))
                velocity = vectorScale(direction, max_speed);

            physicsUpdateTimer = 0;
        }
        ////////////////////////////

        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        
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
        DrawCircle(position.x, position.y, 30, BROWN);
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
