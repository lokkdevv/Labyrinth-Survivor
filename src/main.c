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
    Enemy *enemy = initEnemy(20, 200, 30, (Vector2){30, 30}, 30);
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
        updateAndMovePlayer(player);
        Vector2 player_pos = {player->collisionCircle.circle.x, player->collisionCircle.circle.y};
        updateAndMoveEnemy(enemy, player_pos);

        draw_ui = 0;
        updateObject(object, player);
        updateObject(point1, player);


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
        ClearBackground(RAYWHITE);
        DrawTexture(background_text, 0, 0, WHITE);
        drawPlayer(player);
        drawEnemy(enemy);
        drawObject(object);
        drawObject(point1);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(shader);
        DrawTexturePro(target.texture, srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
        EndShaderMode();
        EndDrawing();
    }
    UnloadRenderTexture(target);
    UnloadShader(shader);
    deleteEnemy(enemy);
    deletePlayer(player);
    deleteObject(object);
    deleteObject(point1);
    CloseWindow();
    return 0;
}
