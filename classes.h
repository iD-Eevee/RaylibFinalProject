#include "raylib.h"

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