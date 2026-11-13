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

#define MAX_ENEMIES 1026
#define ENEMIES_PER_WAVE 5

#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 70

#define BUILD_SLOT_COUNT 6

typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

Rectangle createButton(float y) {
    return (Rectangle){
        GetScreenWidth() / 2.0f - BUTTON_WIDTH / 2.0f,
        y,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    };
}

int buttonPressed(Rectangle button) {
    return CheckCollisionPointRec(GetMousePosition(), button)
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void drawButton(Rectangle button, const char *text) {
    Color color = CheckCollisionPointRec(GetMousePosition(), button)? DARKGRAY : GRAY;
    DrawRectangleRec(button, color);

    int text_width = MeasureText(text, 30);

    DrawText(text, button.x + (button.width - text_width) / 2, button.y + 20, 30, WHITE);
}

/*void restartGame(Player **player, Enemy *enemies[],
    int *wave, int *completed_waves,
    int *enemies_to_spawn, int *enemies_spawned, int *enemies_alive, float *spawn_timer,
    Object *object, Object *point1); */

void restartGame(Player **player, Enemy *enemies[],
    int *wave, int *completed_waves,
    int *enemies_to_spawn, int *enemies_spawned, int *enemies_alive,
    float *spawn_timer, Object *build_slots[]) {
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i] != NULL) {
            deleteEnemy(enemies[i]);
            enemies[i] = NULL;
        }
    }

    deletePlayer(*player);

    *player = initPlayer((Vector2){TARGET_WIDTH / 2, TARGET_HEIGHT / 2},
        ACCELERATION, FRICTION, MAX_SPEED, 1.0f / 60.0f, 30);

    (*player)->money = 100;

    resetTowers();

    for (int i = 0; i < BUILD_SLOT_COUNT; i++) {
        build_slots[i]->type = 2;
        build_slots[i]->interacted = 0;
    }

    *wave = 1;
    *completed_waves = 0;
    *enemies_to_spawn = 5;
    *enemies_spawned = 0;
    *enemies_alive = 0;
    *spawn_timer = 0;

    draw_ui = 0;
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(TARGET_WIDTH, TARGET_HEIGHT, "Labyrinth Survivor");
    SetTargetFPS(90);

    SetExitKey(KEY_NULL);
    GameState game_state = MENU;

    Image background = LoadImage("resources/sprites/Space_Background1.png");
    Texture background_text = LoadTextureFromImage(background);
    UnloadImage(background);

    // Player setup //
    Vector2 startingPos = (Vector2){TARGET_WIDTH/2, TARGET_HEIGHT/2};
    Player *player = initPlayer(startingPos, ACCELERATION, FRICTION, MAX_SPEED, 1.0f/60.0f, 30);
    ///////////////////
    /// Enemy setup ///
    Vector2 targets[7] = {{1520, 350}, {1520, 910}, {960, 910}, {960, 330}, {400, 330}, {400, 690}, {-10, 690}};
    //
    Enemy *enemies[MAX_ENEMIES] = {NULL};
    //
    int wave = 1;
    int completed_waves = 0;
    //
    int enemies_to_spawn = 5;
    int enemies_spawned = 0;
    int enemies_alive = 0;
    //
    float spawn_timer = 0;
    ///////////////////
    /// Object setup //
    Vector2 build_positions[BUILD_SLOT_COUNT] = {{1750, 500}, {1450, 600}, {1350, 1000},
        {900, 700}, {700, 180}, {250, 500}};

    Object *build_slots[BUILD_SLOT_COUNT];
    for (int i = 0; i < BUILD_SLOT_COUNT; i++) {
        build_slots[i] = initObject(build_positions[i], 20);
    }
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
        if (game_state == MENU) {
            Rectangle play_button = createButton(GetScreenHeight() / 2.0f - 40);
            Rectangle exit_button = createButton(GetScreenHeight() / 2.0f + 50);

            if (buttonPressed(play_button)) {
                restartGame(&player, enemies,
                    &wave, &completed_waves,
                    &enemies_to_spawn, &enemies_spawned, &enemies_alive, &spawn_timer,
                    build_slots);

                game_state = PLAYING;
            }

            if (buttonPressed(exit_button))
                break;
        }

        if (game_state == PLAYING) {
            float delta = GetFrameTime();

            updateAndMovePlayer(player);

            spawn_timer += delta;

            if (spawn_timer >= 3.0f &&
                enemies_spawned < enemies_to_spawn) {

                for (int i = 0; i < MAX_ENEMIES; ++i) {
                    if (enemies[i] != NULL)
                        continue;

                    enemies[i] = initEnemy(20, 200, 30, (Vector2){1980, 350}, 30);

                    if (enemies[i] != NULL)
                        enemies_spawned++;
                    break;
                }
                spawn_timer = 0;
            }

            enemies_alive = 0;

            for (int i = 0; i < MAX_ENEMIES; ++i) {
                if (enemies[i] == NULL)
                    continue;

                updateAndMoveEnemy(enemies[i], targets);

                if (enemies[i]->reached_end) {
                    game_state = GAME_OVER;
                    break;
                }
                if (enemies[i]->health <= 0) {
                    player->money += 20;

                    deleteEnemy(enemies[i]);
                    enemies[i] = NULL;
                    continue;
                }
                enemies_alive++;
            }

            if (game_state == PLAYING)
                updateTowers(enemies, MAX_ENEMIES);

            draw_ui = 0;

            for (int i = 0; i < BUILD_SLOT_COUNT; i++) {
                canInteract(player->interactionBox, build_slots[i]);
                updateObject(build_slots[i], player);
            }

            updateUI();

        if (enemies_spawned == enemies_to_spawn && enemies_alive == 0 && game_state == PLAYING) {
            completed_waves++;

            wave++;
            enemies_to_spawn = wave * 5;
            enemies_spawned = 0;
            spawn_timer = 0;
        }
        }
        

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
        ClearBackground(BLACK);
        DrawTexture(background_text, 0, 0, WHITE);
        //DrawText(TextFormat("WAVE %d", wave), 30, 30, 32, WHITE);
        //DrawText(TextFormat("ENEMIES: %d / %d", enemies_spawned, enemies_to_spawn), 30, 70, 20, WHITE);
        DrawText(TextFormat("WAVE %d", wave), 30, 30, 32, WHITE);
        DrawText(TextFormat("ENEMIES: %d / %d", enemies_spawned, enemies_to_spawn), 30, 70, 20, WHITE);
        DrawText(TextFormat("MONEY: $%d", player->money), 30, 110, 24, YELLOW);
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
        for (int i = 0; i<MAX_ENEMIES; ++i) {
            if (enemies[i] == NULL)
                continue;
            drawEnemy(enemies[i]);
        }

        drawTowers();
        for (int i = 0; i < BUILD_SLOT_COUNT; i++) {
            drawObject(build_slots[i]);
        }
        drawBuildUI();

        EndTextureMode();

        // Not Important //
        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(shader);
        DrawTexturePro(target.texture, srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        EndShaderMode();
        if (game_state == MENU) {
            DrawRectangle(0, 0,
                GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 230});

            const char *title = "LABYRINTH SURVIVOR";
            const char *credit = "Background art made by digitalmoons.itch.io (Not affiliated with the game)";
            const char *more_credit = "Programming by lokk";

            DrawText(more_credit,
                GetScreenWidth() / 2 - MeasureText(more_credit, 18)/ 2,
                GetScreenHeight() - 20, 18, LIGHTGRAY);
            DrawText(credit,
                GetScreenWidth() / 2 - MeasureText(credit, 18)/ 2,
                GetScreenHeight() - 40, 18, LIGHTGRAY);
            DrawText(title,
                GetScreenWidth() / 2 - MeasureText(title, 60) / 2,
                GetScreenHeight() / 2 - 180, 60, WHITE);

            drawButton(createButton(GetScreenHeight() / 2.0f - 40), "PLAY");
            drawButton(createButton(GetScreenHeight() / 2.0f + 50), "EXIT");
        }

        if (game_state == GAME_OVER) {
            DrawRectangle(0, 0,
                GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 230});

            const char *title = "GAME OVER";

            DrawText(title,
                GetScreenWidth() / 2 - MeasureText(title, 70) / 2,
                GetScreenHeight() / 2 - 200, 70, RED);

            DrawText(
                TextFormat("Waves survived: %d", completed_waves),
                GetScreenWidth() / 2 - 150,
                GetScreenHeight() / 2 - 90,
                30,
                WHITE
            );

            drawButton(createButton(GetScreenHeight() / 2.0f), "RESTART");
            drawButton(createButton(GetScreenHeight() / 2.0f + 90), "MAIN MENU");
            if (buttonPressed(createButton(GetScreenHeight() / 2.0f))) {
                restartGame(&player, enemies,
                    &wave, &completed_waves,
                    &enemies_to_spawn, &enemies_spawned, &enemies_alive, &spawn_timer,
                    build_slots);

                game_state = PLAYING;
            }

            if (buttonPressed(createButton(GetScreenHeight() / 2.0f + 90)))
                game_state = MENU;
        }
        EndDrawing();
        ///////////////////
    }
    UnloadRenderTexture(target);
    UnloadTexture(background_text);
    UnloadShader(shader);
    deletePlayer(player);
    for (int i = 0; i < BUILD_SLOT_COUNT; i++) {
        deleteObject(build_slots[i]);
    }
    for (int i = 0; i<MAX_ENEMIES; ++i) {
        if (enemies[i] != NULL)
            deleteEnemy(enemies[i]);
    }
    CloseWindow();
    return 0;
}

