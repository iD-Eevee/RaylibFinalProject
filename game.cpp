
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
    float speed = 4.0f;

    // Player Direction
    Texture2D playerRight = LoadTexture("images/player_right.png");
    Texture2D playerLeft = LoadTexture("images/player_left.png");
    Texture2D playerUp = LoadTexture("images/player_up.png");
    Texture2D playerDown = LoadTexture("images/player_down.png");
    typedef enum
    {
        up,
        down,
        left,
        right
    } Direction;
    Direction playerDir = down;

    SetTargetFPS(60);
    // ================================================================================================================
    // Game Loop
    while (!WindowShouldClose())
    {
        //-------------------------------------------------
        // Player Controls
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            player.x += speed;
            playerDir = right;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            player.x -= speed;
            playerDir = left;
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            player.y -= speed;
            playerDir = up;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            player.y += speed;
            playerDir = down;
        }

        if ((player.x + 64) >= screenWidth) // Right
        {
            player.x = (screenWidth - 64);
        }
        if (player.x <= 0) // Left
        {
            player.x = 0;
        }
        if (player.y <= 0) // Up
        {
            player.y = 0;
        }
        if ((player.y + 64) >= screenHeight) // Down
        {
            player.y = (screenHeight - 64);
        }

        //-------------------------------------------------
        BeginDrawing();

        // Draw Background
        DrawTexture(bgImage, 0, 0, WHITE);

        // Draw the Player
        switch (playerDir)
        {
            case right:
                DrawTexture(playerRight, player.x, player.y, WHITE);
            break;
            case left:
                DrawTexture(playerLeft, player.x, player.y, WHITE);
            break;
            case up:
                DrawTexture(playerUp, player.x, player.y, WHITE);
            break;
            case down:
                DrawTexture(playerDown, player.x, player.y, WHITE);
            break;
            default:
            break;
        }

        EndDrawing();
        //-------------------------------------------------
    }
    CloseWindow();
}
