#ifndef PROP_H
#define PROP_H

#include "raylib.h"

class Prop
{
    public:
        Prop(Vector2 worldPos, Texture2D tex);
        void render(Vector2 knightWorldPos);
        Rectangle getCollisionRectangle(Vector2 knightWorldPos);
    private:
        Vector2 worldPos {};
        Texture2D tex;
        float scale {4.0f};
};

#endif