#include "raylib.h"

enum Type
{
    Moving,
    Idle
};

enum Movement
{
    X,
    Y
};

class Item
{
    public:
    Texture2D image = LoadTexture("images/coin.png");
    Vector2 position;
    Rectangle collider;
    bool collect = false;
    float size = 54;

    Item(float x, float y)
    {
        position = {x,y};
        collider = {x, y, size, size};
    }
};

class Enemy
{
    public:
    Texture2D image;
    Vector2 position;
    Rectangle collider;
    bool destroyed = false;
    float size = 54;
    const char *enemyImage;
    float speed, start, end, velocity;
    Movement direction;

    Enemy(float x, float y, Type state = Idle, Movement dir = X, float startPos = 100, float endPos = 300, float enemySpeed = 3)
    {
        switch(state)
        {
            case Moving:
            enemyImage = "images/ghost.png";
            speed = enemySpeed;
            start = startPos;
            end = endPos;
            direction = dir;
            break;

            case Idle:
            enemyImage = "images/enemy.png";
            speed = 0;
            start = x;
            end = x;
            break;

            default:
            break;
        }
        image = LoadTexture(enemyImage);
        position = {x,y};
        collider = {x, y, size, size};
    }
};

class Wall
{
    public:
    Texture2D image = LoadTexture("images/wall.png");
    Vector2 position;
    Rectangle collider;
    float size = 64;

    Wall(float x, float y)
    {
        position = {x,y};
        collider = {x, y, size, size};
    }
};
