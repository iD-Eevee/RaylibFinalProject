
// DONUT LAND INVASION
// A Top-Down Action-Adventure Game
// Created by Eevee
//
// MUSIC CREDITS:
// Blue Ska & Mesmerizing Galaxy
// by Kevin MacLeod (incompetech.com)
// Licensed under Creative Commons: By Attribution 3.0 License
// http://creativecommons.org/licenses/by/3.0/

#include "raylib.h"
#include "classes.h"

// ===================================================================================================================
int main()
{
    // Initialize Window
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Evo in Donut Land");

    // Variables
    int score = 0;

    // Scenes
    typedef enum
    {
        title,
        level,
        end,
        gameOver
    } Scene;
    Scene scene = title;

    // Level
    typedef enum
    {
        donutLand,
        swamp
    } Level;
    Level currentLevel = donutLand;
    bool initLevel = false;

    // Initialize Background
    Texture2D bgImage = LoadTexture("images/level1.png");
    Texture2D bgImageSwamp = LoadTexture("images/level2.png");

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
    Rectangle goalCollider = {goal.x, goal.y, 100, 100};
    bool goalCollision = false;
    bool restart = false;

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

    // Music
    InitAudioDevice();
    Music donutMusic = LoadMusicStream("sounds/Blue Ska.mp3");
    Music swampMusic = LoadMusicStream("sounds/Mesmerizing Galaxy Loop.mp3");
    Music levelMusic = donutMusic;
    PlayMusicStream(levelMusic);

    // Sound Effects
    Sound attackSound = LoadSound("sounds/sndPop2.mp3");

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
                // Music
                if (initLevel == false)
                {
                    StopMusicStream(levelMusic);
                }
                UpdateMusicStream(levelMusic);
                
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
            // End
            case end:
            if (IsKeyPressed(KEY_R))
            {
                scene = title;
            }
            break;

            //====================================================================
            // Game Over
            case gameOver:
            if (IsKeyPressed(KEY_R))
            {
                scene = title;
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
            DrawText("Evo in Donut Land", 80, 150, 70, BLACK);
            DrawText("Press Enter to Start", 230, 250, 30, BLACK);
            restart = true;
            currentLevel = donutLand;
            initLevel = false;
            if (restart == true)
            {
                /*player.x = playerX;
                player.y = playerY;
                playerCollider.x = player.x;
                playerCollider.y = player.y;*/
                playerHealth = playerHealthMax;
                score = 0;
                restart = false;
            }
            break;

            //====================================================================
            // Level
            case level:
            // Different Levels
            switch (currentLevel)
            {
                //====================================================================
                // Level 1 (Donut Land)
                case donutLand:
                // Draw Background
                DrawTexture(bgImage, 0, 0, WHITE);
                
                // Music
                levelMusic = donutMusic;

                if (initLevel == false)
                {
                    // Player Position
                    playerX = 550;
                    playerY = 80;

                    // Collectibles
                    items[0] = Item(100, 100);
                    items[1] = Item(100, 200);
                    items[2] = Item(100, 300);

                    // Enemies
                    enemies[0] = {Enemy(500, 200, Idle)};
                    enemies[1] = {Enemy(400, 200)};
                    enemies[2] = {Enemy(150, 100, Moving, X, 100, 300)};
                    enemies[3] = {Enemy(250, 200, Moving, Y, 200, 300)};

                    // NPCs
                    npcImage = LoadTexture("images/warbler.png");
                    npc.x = 330;
                    npc.y = 290;

                    // Goal
                    goal.x = 650;
                    goal.y = 300;

                    // Key
                    key.x = 350;
                    key.y = 150;

                    // Walls
                    walls[0] = {Wall(10,200)};
                    walls[1] = {Wall(10,300)};
                }
                break;

                //====================================================================
                // Level 2 (Swamp)
                case swamp:
                // Draw Background
                DrawTexture(bgImageSwamp, 0, 0, WHITE);

                // Music
                levelMusic = swampMusic;

                if (initLevel == false)
                {
                    // Player Position
                    playerX = 20;
                    playerY = 100;

                    // Collectibles
                    items[0] = Item(30, 360);
                    items[1] = Item(200, 210);
                    items[2] = Item(510, 130);

                    // Enemies
                    enemies[0] = {Enemy(30, 250)};
                    enemies[1] = {Enemy(360, 240)};
                    enemies[2] = {Enemy(260, 340, Moving, X, 260, 550)};
                    enemies[3] = {Enemy(720, 70, Moving, Y, 70, 250)};

                    // NPCs
                    npcImage = LoadTexture("images/flamingo.png");
                    npc.x = 320;
                    npc.y = 120;

                    // Key
                    key.x = 290;
                    key.y = 60;

                    // Goal
                    goal.x = 650;
                    goal.y = 300;

                    // Walls
                    walls[0] = {Wall(900,900)};
                    walls[1] = {Wall(900,900)};
                }
                break;

                default:
                break;
            }

            // Initialization for all Levels
            if (initLevel == false)
            {
                // Set Colliders & Positions
                player.x = playerX;
                player.y = playerY;
                playerCollider.x = player.x;
                playerCollider.y = player.y;
                goalCollider.x = goal.x;
                goalCollider.y = goal.y;
                keyCollider.x = key.x;
                keyCollider.y = key.y;
                npcCollider.x = npc.x;
                npcCollider.y = npc.y;
                restart = false;

                // Reset Coins Collected
                for (int i = 0; i < 3; ++i)
                {
                    items[i].collect = false;
                }

                // Reset Enemies Destroyed
                for (int i = 0; i < 4; ++i)
                {
                    enemies[i].destroyed = false;
                }

                // Music
                PlayMusicStream(levelMusic);

                // Complete Inialization
                initLevel = true;
            }

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
                        PlaySound(attackSound);
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
                            //gameEnd = true;
                            scene = gameOver;
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
                goalCollision = CheckCollisionRecs(playerCollider, goalCollider);
                if (goalCollision == true)
                {
                    unlocked = false;
                    initLevel = false;

                    // Set the Destination
                    if (currentLevel == swamp)
                    {
                        scene = end;
                    }
                    else if (currentLevel == donutLand)
                    {
                        currentLevel = swamp;
                    }
                }
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
                    // Warbler
                    if (currentLevel == donutLand)
                    {
                        dialogue = "Do you have any hints?";
                    }
                    // Flamingo
                    else if (currentLevel == swamp)
                    {
                        dialogue = "Do you live in this swamp?";
                    }
                    lines = 1;
                    speaker = player;
                    speakerWidth = playerRight.width;
                }
                else if (currentLine == 2)
                {
                    // Warbler
                    if (currentLevel == donutLand)
                    {
                        dialogue = "Collect the key to find \nthe secret ending donut.";
                    }
                    // Flamingo
                    else if (currentLevel == swamp)
                    {
                        dialogue = "Collect the key to find \nthe secret ending donut.";
                        dialogue = "I just moved here, yeah. \nI came from the beach!";
                    }
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
                    DrawText(dialogue, textBoxX + padding, textBoxY + (padding * 4 ) + speakerWidth, textSize, BLACK);
                }
            }
            break;

            //====================================================================
            // End
            case end:
            ClearBackground(GREEN);
            DrawText("You Win!", 250, 150, 70, BLACK);
            DrawText("Press R to Return to Title", 200, 250, 30, BLACK);
            break;

            //====================================================================
            // Game Over
            case gameOver:
            ClearBackground(RED);
            DrawText("GAME OVER", 200, 150, 70, BLACK);
            DrawText("Press R to Return to Title", 200, 250, 30, BLACK);
            break;

            //====================================================================
            // Default
            default:
            break;
        }

        EndDrawing();
        //-------------------------------------------------
    }
    // Unload Music
    UnloadMusicStream(levelMusic);
    CloseAudioDevice();

    CloseWindow();
}
