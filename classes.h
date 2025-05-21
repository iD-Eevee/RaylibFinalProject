#include "raylib.h"

enum Type
{
    Moving,
    Idle
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

    Enemy(float x, float y, Type state = Idle)
    {
        switch(state)
        {
            case Moving:
            enemyImage = "images/ghost.png";
            break;

            case Idle:
            enemyImage = "images/enemy.png";
            break;

            default:
            break;
        }
        image = LoadTexture(enemyImage);
        position = {x,y};
        collider = {x, y, size, size};
    }
};