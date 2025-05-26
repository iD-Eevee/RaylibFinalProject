
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

    // Scenes
    typedef enum
    {
        title,
        level
    } Scene;
    Scene scene = title;

    // Initialize Background
    Texture2D bgImage = LoadTexture("images/level1.png");

    // Initialize Player
    //Texture2D playerImage = LoadTexture("images/player.png");
    float playerX = 550;
    float playerY = 80;
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

    // Health
    float playerHealth = 100;
    Color playerColor = WHITE;
    float playerHealthMax = 100;
    float healthWidth;
    float healthX;
    Color healthColor = GREEN;

    // Key Item Initialization
    Texture2D keyImage = LoadTexture("images/key.png");
    Vector2 key = {350, 150};
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

    // Initialize Walls
    bool wallCollision = false;
    Wall walls[2] = {Wall(10,200), Wall(10,300)};

    // Initialize NPC
    Texture2D npcImage = LoadTexture("images/warbler.png");
    Vector2 npc = {330, 290};
    Rectangle npcCollider = {npc.x, npc.y, 100, 100};
    float npcCollision = false;

    // Dialogue
    const char *dialogue;
    float dialogueLength, textBoxWidth, textBoxHeight, textBoxX, textBoxY;
    int textSize = 20;
    float padding = 10;
    int lines;
    int currentLine = 0;
    Vector2 speaker;
    float speakerWidth;
    bool dialogueActive = false;

    // Cooldown & Timers
    double startTime = GetTime();
    double currentTime = 0.0;
    double cooldownTime = 1;
    bool cooldown = false;

    SetTargetFPS(60);
    // ================================================================================================================
    // Game Loop
    while (!WindowShouldClose() && !gameEnd)
    {
        //================================================================================================
        // Scenes
        switch (scene)
        {
            //====================================================================
            // Title
            case title:
            if (IsKeyPressed(KEY_ENTER))
            {
                scene = level;
            }
            break;

            //====================================================================
            // Level
            case level:
                //-------------------------------------------------
                // Player Controls
                if (dialogueActive == false)
                {
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
                }
                
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

                //----------------------
                // Current Dialogue Line
                if (npcCollision == true)
                {
                    if (IsKeyPressed(KEY_X))
                    {
                        dialogueActive = true;
                        if (currentLine < 2)
                        {
                            currentLine += 1;
                        }
                        else
                        {
                            dialogueActive = false;
                            currentLine = 0;
                        }
                    }
                }
            break;

            //====================================================================
            // Default
            default:
            break;
        }
        
        //================================================================================================
        BeginDrawing();

        //================================================================================================
        // Scenes
        switch (scene)
        {
            //====================================================================
            // Title
            case title:
            ClearBackground(ORANGE);
            DrawText("Donut Land Invasion", 40, 150, 70, BLACK);
            DrawText("Press Enter to Start", 230, 250, 30, BLACK);
            break;

            //====================================================================
            // Level
            case level:
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
            // Wall Collision
            for (int i = 0; i < 2; ++i)
            {
                DrawTexture(walls[i].image, walls[i].position.x, walls[i].position.y, WHITE);
                wallCollision = CheckCollisionRecs(playerCollider, walls[i].collider);
                if (wallCollision == true)
                {
                    switch (playerDir)
                    {
                        case up:
                            player.y = walls[i].position.y + 64;
                        break;
                            
                        case down:
                            player.y = walls[i].position.y - 64;
                        break;

                        case left:
                            player.x = walls[i].position.x + 64;
                        break;

                        case right:
                            player.x = walls[i].position.x - 64;
                        break;

                        default:
                        break;
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
                        // Cooldown Timer
                        cooldown = true;
                        if (currentTime >= cooldownTime)
                        {
                            playerHealth -= 20;
                            startTime = GetTime();
                        }
                
                        // Game Over
                        if (playerHealth <= 0)
                        {
                            gameEnd = true;
                        }
                    }
                }
            }
            //----------------------
            // Cooldown Timer
            if (cooldown == true)
            {
                if (currentTime >= cooldownTime)
                {
                    cooldown = false;
                }
                currentTime = GetTime() - startTime;
                playerColor = RED;
            }
            else
            {
                playerColor = WHITE;
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

            //----------------------
            // Draw Healthbar
            healthWidth = (100 * playerHealth) / playerHealthMax;
            healthX = player.x + (playerRight.width / 2) - (healthWidth / 2);

            // Healthbar Color
            if (playerHealth <= 25)
            {
                healthColor = RED;
            }
            else if (playerHealth <= 50)
            {
                healthColor = ORANGE;
            }
            else if (playerHealth <= 75)
            {
                healthColor = YELLOW;
            }
            else
            {
                healthColor = GREEN;
            }
            DrawRectangle(healthX, player.y - 5, healthWidth, 10, healthColor);

            //----------------------
            // Draw the Player
            switch (playerDir)
            {
                case right:
                    DrawTexture(playerRight, player.x, player.y, playerColor);
                break;
                case left:
                    DrawTexture(playerLeft, player.x, player.y, playerColor);
                break;
                case up:
                    DrawTexture(playerUp, player.x, player.y, playerColor);
                break;
                case down:
                    DrawTexture(playerDown, player.x, player.y, playerColor);
                break;
                default:
                break;
            }

            //----------------------
            // Draw the NPC
            DrawTexture(npcImage, npc.x, npc.y, WHITE);
            npcCollision = CheckCollisionRecs(playerCollider, npcCollider);
            if (npcCollision == true)
            {
                // Dialogue Lines
                if (currentLine == 1)
                {
                    dialogue = "Do you have any hints?";
                    lines = 1;
                    speaker = player;
                    speakerWidth = playerRight.width;
                }
                else if (currentLine == 2)
                {
                    dialogue = "Collect the key to find \nthe secret ending donut.";
                    lines = 2;
                    speaker = npc;
                    speakerWidth = npcImage.width;
                }
                else
                {
                    dialogue = "Press X to Interact";
                    lines = 1;
                    speaker = npc;
                    speakerWidth = npcImage.width;
                }

                // Text Box Setup
                dialogueLength = MeasureText(dialogue, textSize);
                textBoxWidth = dialogueLength + (padding * 2);
                textBoxX = speaker.x + (speakerWidth / 2) - (textBoxWidth / 2);
                textBoxHeight = (textSize + padding) * lines;
                textBoxY = speaker.y - textBoxHeight - padding;

                // Draw the Dialogue
                if (currentLine != 0)
                {
                    DrawRectangle(textBoxX, textBoxY, textBoxWidth, textBoxHeight, BLUE);
                    DrawText(dialogue, textBoxX + padding, textBoxY + padding, textSize, WHITE);
                }
                else
                {
                    DrawText(dialogue, textBoxX + padding, textBoxY + (padding * 4 ) + speakerWidth, textSize, GREEN);
                }
            }
            break;

            //====================================================================
            // Default
            default:
            break;
        }

        EndDrawing();
        //-------------------------------------------------
    }
    CloseWindow();
}
