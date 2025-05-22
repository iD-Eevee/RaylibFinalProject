
// DONUT LAND INVASION
// A Top-Down Action-Adventure Game
// Created by Eevee

#include "raylib.h"
#include "classes.h"

// ===================================================================================================================
int main()
{
    // Initialize Window
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Top-Down Game");

    // Variables
    int score = 0;

    // Initialize Background
    Texture2D bgImage = LoadTexture("images/level1.png");

    // Initialize Player
    //Texture2D playerImage = LoadTexture("images/player.png");
    float playerX = 500;
    float playerY = 100;
    Vector2 player = {playerX, playerY};
    float speed = 4.0f;
    Rectangle playerCollider = {player.x, player.y, 40, 40};

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

    // Player Combat
    Texture2D attackImage = LoadTexture("images/fire.png");
    Vector2 attack = {player.x, player.y};
    Rectangle attackCollider = {attack.x, attack.y, 32, 32};
    bool attackCollision = false;
    bool attacking = false;

    // Key Item Initialization
    Texture2D keyImage = LoadTexture("images/key.png");
    Vector2 key = {350, 200};
    Rectangle keyCollider = {key.x, key.y, 50, 50};
    bool keyCollision = false;
    bool unlocked = false;

    // Goal Initialization
    Texture2D goalImage = LoadTexture("images/goal.png");
    Vector2 goal = {650, 300};

    // Collectible Item Initialization
    bool coinCollision = false;
    Item items[3] = {Item(100, 100), Item(100, 200), Item(100, 300)};

    // Enemy Initialization
    Enemy enemies[4] = {Enemy(500, 200, Idle),
        Enemy(400, 200),
        Enemy(150, 100, Moving, X, 100, 300),
        Enemy(250, 200, Moving, Y, 200, 300)};
    bool enemyCollision = false;
    bool gameEnd = false;

    SetTargetFPS(60);
    // ================================================================================================================
    // Game Loop
    while (!WindowShouldClose() && !gameEnd)
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
        // Set the Collider's Position
        playerCollider.x = player.x;
        playerCollider.y = player.y;

        //----------------------
        // Boundaries for the Player Movement
        if ((player.x + 64) >= screenWidth) // Right
        {
            player.x = (screenWidth - 64);
        }
        if (player.x <= 0) // Left
        {
            player.x = 0;
        }
        if (player.y <= 50) // Up
        {
            player.y = 50;
        }
        if ((player.y + 64) >= screenHeight) // Down
        {
            player.y = (screenHeight - 64);
        }

        //----------------------
        // Key Collision
        if (unlocked == false)
        {
            keyCollision = CheckCollisionRecs(playerCollider, keyCollider);
            if (keyCollision == true)
            {
                unlocked = true;
            }
        }

        //----------------------
        // Player Attack
        if (IsKeyPressed(KEY_Z))
        {
            if (attacking == false)
            {
                attacking = true;
                switch (playerDir)
                {
                    case up:
                        attack.x = player.x + 10;
                        attack.y = player.y - 30;
                    break;

                    case down:
                        attack.x = player.x + 10;
                        attack.y = player.y + 60;
                    break;

                    case left:
                        attack.x = player.x - 30;
                        attack.y = player.y + 10;
                    break;

                    case right:
                        attack.x = player.x + 60;
                        attack.y = player.y + 10;
                    break;

                    default:
                    break;
                }
                attackCollider.x = attack.x;
                attackCollider.y = attack.y;
            }
        }
        else
        {
            attacking = false;
        }
        
        //-------------------------------------------------
        BeginDrawing();

        // Draw Background
        DrawTexture(bgImage, 0, 0, WHITE);

        //----------------------
        // Coin Collision
        for (int i = 0; i < 3; ++i)
        {
            if (items[i].collect == false)
            {
                coinCollision = CheckCollisionRecs(playerCollider, items[i].collider);
                if (coinCollision == true)
                {
                    score += 10;
                    items[i].collect = true;
                }
                else
                {
                    DrawTexture(items[i].image, items[i].position.x, items[i].position.y, WHITE);
                }
            }
        }
        
        //----------------------
        // Enemies
        for (int i = 0; i < 4; ++i)
        {
            if (enemies[i].destroyed == false)
            {
                enemyCollision = CheckCollisionRecs(enemies[i].collider, playerCollider);
                attackCollision = CheckCollisionRecs(enemies[i].collider, attackCollider);
                // Collision with Attack
                if (attackCollision == true)
                {
                    enemies[i].destroyed = true;
                }
                else
                {
                    // Draw Enemies
                    DrawTexture(enemies[i].image, enemies[i].position.x, enemies[i].position.y, WHITE);

                    // Move Enemies
                    enemies[i].velocity = enemies[i].speed;
                    if (enemies[i].direction == X) 
                    {
                        enemies[i].position.x += enemies[i].velocity;
                        if (enemies[i].position.x <= enemies[i].start || enemies[i].position.x >= enemies[i].end)
                        {
                            enemies[i].speed = -enemies[i].velocity;
                        }
                    }
                    else
                    {
                        enemies[i].position.y += enemies[i].velocity;
                        if (enemies[i].position.y <= enemies[i].start || enemies[i].position.y >= enemies[i].end)
                        {
                            enemies[i].speed = -enemies[i].velocity;
                        }
                    }
                    
                    enemies[i].collider.x = enemies[i].position.x;
                    enemies[i].collider.y = enemies[i].position.y;
                }

                // Collision with Player
                if (enemyCollision == true)
                {
                    gameEnd = true;
                }
            }
        }

        //----------------------
        // Draw the Attack
        if (attacking == true)
        {
            DrawTexture(attackImage, attack.x, attack.y, WHITE);
        }

        //----------------------
        // Draw the Key
        if (unlocked == false)
        {
            DrawTexture(keyImage, key.x, key.y, WHITE);
        }
        else
        {
            DrawTexture(goalImage, goal.x, goal.y, WHITE);
        }

        //----------------------
        // Draw Score
        DrawRectangle(0, 0, screenWidth, 50, BLACK);
        DrawText(TextFormat("Score: %i", score), 10, 10, 30, WHITE);

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
