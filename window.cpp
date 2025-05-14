#include "raylib.h"

int main()
{
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Project");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);
        DrawText("Welcome to my first Raylib project!", 100, 220, 30, LIGHTGRAY);

        EndDrawing();
    }
    CloseWindow();
}
