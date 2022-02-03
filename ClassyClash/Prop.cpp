#include "Prop.h"

#include "raymath.h"

Prop::Prop(Vector2 worldPos, Texture2D tex) :
worldPos{worldPos}, tex{tex}
{}

void Prop::render(Vector2 knightWorldPos)
{
    // Just like the map itself, props also have to move in the opposite direction
    // of the player. So their screen position has to be adjusted accordingly. The world pos
    // defines where in the world or in the map an object is locared. Screen pos defines where
    // in the screen we have to draw the object. When it comes to props, we want them to stay
    // wherever we've put them in the map as it is part of the level design. However, they have to
    // change their screen position as the player moves around. We can accomplish this either by
    // adding the map pos vector to the world pos vector or by subtracting the knight's world pos
    // vector from the world pos vector of the prop itself. Note that there operations are basically
    // the same as the map pos is just the inverse of the knight's world pos
    Vector2 screenPos = Vector2Subtract(worldPos, knightWorldPos);

    DrawTextureEx(tex, screenPos, 0.0f, scale, WHITE);
}

Rectangle Prop::getCollisionRectangle(Vector2 knightWorldPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, knightWorldPos);

    return Rectangle {
        screenPos.x,
        screenPos.y,
        scale * tex.width,
        scale * tex.height
    };    
}