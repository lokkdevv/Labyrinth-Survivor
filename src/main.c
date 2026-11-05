#include <raylib.h>

#define windowWidth 800
#define windowHeight 600

typedef struct{
    int x;
    int y;
    int gripping;
} hand;

typedef struct{
    int x;
    int y;
    int health;
} body;

int main()
{
    InitWindow(windowWidth, windowHeight, "Candy Distribution");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    hand hand = {};
    body body = {};
    Vector2 movement = {};
    Rectangle bean = {0, 0, 50, 20};

    while (!WindowShouldClose()) {
        hand.x = GetMouseX();
        hand.y = GetMouseY();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            hand.gripping = 1;
        else
            hand.gripping = 0;

        if (hand.gripping == 1) {
            movement.x = hand.x - bean.x;
            movement.y = hand.y - bean.y;
        }
        else
            movement = (Vector2){0,0};

        bean.x += movement.x;
    bean.y += movement.y;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectanglePro(bean, (Vector2){bean.height/2,bean.width/2}, 0, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
