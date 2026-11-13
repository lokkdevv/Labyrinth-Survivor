#include <raylib.h>
#include "../include/utils.h"
#include "../include/collision.h"
#include "../include/player.h"
#include "../include/enemy.h"
#include "../include/object.h"

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

#define ACCELERATION 10.0f
#define FRICTION 7.0f
#define MAX_SPEED 10000.0f

#define LIGHTPURPLE (Color){112, 41, 99, 20}

#define MAX_ENEMIES 1026
#define SPAWN_INTERVAL 3.0f
#define ENEMIES_PER_WAVE 5

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(TARGET_WIDTH, TARGET_HEIGHT, "Labyrinth Survivor");
    SetTargetFPS(90);

    Image background = LoadImage("resources/sprites/Space_Background1.png");
    Texture background_text = LoadTextureFromImage(background);
    UnloadImage(background);

    // Player setup //
    Vector2 startingPos = (Vector2){TARGET_WIDTH/2, TARGET_HEIGHT/2};
    Player *player = initPlayer(startingPos, ACCELERATION, FRICTION, MAX_SPEED, 1.0f/60.0f, 30);
    ///////////////////
    /// Enemy setup ///
    Vector2 targets[7] = {{1520, 350}, {1520, 910}, {960, 910}, {960, 330}, {400, 330}, {400, 690}, {-10, 690}};

    Enemy *enemies[MAX_ENEMIES] = {NULL};
    int wave = 1;
    int enemies_to_spawn = wave * ENEMIES_PER_WAVE;
    int enemies_spawned = 0;
    int enemies_alive = 0;

    float spawn_timer = SPAWN_INTERVAL;

    ///////////////////
    /// Object setup //
    Object *object = initObject((Vector2){600, 300}, 20);
    Object *point1 = initObject((Vector2){1920-200, 500}, 20);
    ///////////////////

    // BoilerPlate code //
    RenderTexture2D target = LoadRenderTexture(TARGET_WIDTH, TARGET_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
    Shader shader = LoadShader(0, "resources/shaders/shader.fs");
    //if (shader.id == 0)

    float scale;
    Rectangle srcRect;
    Rectangle destRect;

    double delta;
    //////////////////////

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        spawn_timer += delta;

        while (spawn_timer >= SPAWN_INTERVAL && enemies_spawned < enemies_to_spawn) {
            for (int i = 0; i < MAX_ENEMIES; ++i) {
                if (enemies[i] != NULL)
                    continue;

                enemies[i] = initEnemy(20, 200, 30, (Vector2){1980, 350}, 30, 100);

                if (enemies[i] != NULL)
                    enemies_spawned++;

                break;
            }
            spawn_timer -= SPAWN_INTERVAL;
        }

        updateAndMovePlayer(player);
        Vector2 player_pos = {player->collisionCircle.circle.x, player->collisionCircle.circle.y};

        enemies_alive = 0;
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            if (enemies[i] == NULL)
                continue;

            updateAndMoveEnemy(enemies[i], targets);

            if (enemies[i]->health <= 0) {
                deleteEnemy(enemies[i]);
                enemies[i] = NULL;
                continue;
            }
            enemies_alive++;
        }
        if (enemies_spawned >= enemies_to_spawn && enemies_alive == 0) {
            wave++;

            enemies_to_spawn = wave * ENEMIES_PER_WAVE;
            enemies_spawned = 0;

            spawn_timer = 0;
        }

        draw_ui = 0;
        for (int i = 0; i<MAX_ENEMIES; ++i) {
            if (enemies[i] == NULL)
                continue;
            updateObject(object, player, enemies[i]);
            updateObject(point1, player, enemies[i]);
        }

        canInteract(player->interactionBox, object);
        canInteract(player->interactionBox, point1);

        ////// BoilerPlate //////
        scale = minf((float)GetScreenWidth()/TARGET_WIDTH, (float)GetScreenHeight()/TARGET_HEIGHT);
        srcRect = (Rectangle){0, 0, (float)TARGET_WIDTH, (float)-TARGET_HEIGHT};
        destRect = (Rectangle){(float)(GetScreenWidth()-(TARGET_WIDTH*scale))*0.5,
                            (float)(GetScreenHeight()-(TARGET_HEIGHT*scale))*0.5,
                            (float)TARGET_WIDTH*scale,
                            (float)TARGET_HEIGHT*scale
                            };
        /////////////////////////

        updateUI();
        
        BeginTextureMode(target);
        DrawText(TextFormat("WAVE %d", wave), 30, 30, 32, WHITE);
        DrawText(TextFormat("ENEMIES: %d / %d", enemies_spawned, enemies_to_spawn), 30, 70, 20, WHITE);
        ClearBackground(BLACK);
        DrawTexture(background_text, 0, 0, WHITE);
        // Path Drawing //
        DrawRectangleRounded((Rectangle){1460, 290, 700, 120}, 1, 2, LIGHTPURPLE);
        DrawRectangleRounded((Rectangle){1460, 290, 120, 680}, 1, 2, LIGHTPURPLE);
        DrawRectangleRounded((Rectangle){900, 850, 680, 120}, 1, 2, LIGHTPURPLE);
        DrawRectangleRounded((Rectangle){900, 270, 120, 700}, 1, 2, LIGHTPURPLE);
        DrawRectangleRounded((Rectangle){340, 270, 680, 120}, 1, 2, LIGHTPURPLE);
        DrawRectangleRounded((Rectangle){340, 270, 120, 480}, 1, 2, LIGHTPURPLE);
        DrawRectangleRounded((Rectangle){-50, 630, 510, 120}, 1, 2, LIGHTPURPLE);
        //////////////////
        // Target Debug Drawing //
        DrawCircle(1520, 350, 5, debugYellow);
        DrawCircle(1520, 910, 5, debugYellow);
        DrawCircle(960, 910, 5, debugYellow);
        DrawCircle(960, 330, 5, debugYellow);
        DrawCircle(400, 330, 5, debugYellow);
        DrawCircle(400, 690, 5, debugYellow);
        DrawCircle(-10, 690, 5, debugYellow);
        //////////////////////////

        drawPlayer(player);
        drawObject(object);
        drawObject(point1);

        for (int i = 0; i<MAX_ENEMIES; ++i) {
            if (enemies[i] == NULL)
                continue;
            drawEnemy(enemies[i]);
        }

        EndTextureMode();

        // Not Important //
        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(shader);
        DrawTexturePro(target.texture, srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        EndShaderMode();
        EndDrawing();
        ///////////////////
    }
    UnloadRenderTexture(target);
    UnloadTexture(background_text);
    UnloadShader(shader);
    deletePlayer(player);
    deleteObject(object);
    deleteObject(point1);
    for (int i = 0; i<MAX_ENEMIES; ++i) {
        if (enemies[i] != NULL)
            deleteEnemy(enemies[i]);
    }
    CloseWindow();
    return 0;
}
