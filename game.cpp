
// DONUT LAND INVASION
// A Top-Down Action-Adventure Game
// Created by Eevee

#include "raylib.h"

// ===================================================================================================================
int main()
{
    // Initialize Window
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Top-Down Game");

    // Initialize Background
    Texture2D bgImage = LoadTexture("images/level1.png");

    // Initialize Player
    Texture2D playerImage = LoadTexture("images/player.png");
    float playerX = 500;
    float playerY = 100;
    Vector2 player = {playerX, playerY};

    SetTargetFPS(60);
    // ================================================================================================================
    // Game Loop
    while (!WindowShouldClose())
    {
        //-------------------------------------------------
        BeginDrawing();

        // Draw Background
        DrawTexture(bgImage, 0, 0, WHITE);

        // Draw the Player
        DrawTexture(playerImage, player.x, player.y, WHITE);

        EndDrawing();
        //-------------------------------------------------
    }
    CloseWindow();
}
