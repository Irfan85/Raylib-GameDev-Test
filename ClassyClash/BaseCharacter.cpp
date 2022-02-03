#include "BaseCharacter.h"

#include "raymath.h"

BaseCharacter::BaseCharacter(Texture2D idleTexture, Texture2D runningTexture, int animMaxFrame)
: texture{idleTexture}, idleTexture{idleTexture}, runningTexture{runningTexture}, animMaxFrame{animMaxFrame}
{
    width = texture.width / animMaxFrame;
    height = texture.height;
}

Vector2 BaseCharacter::getWorldPos()
{
    return worldPos;
}

bool BaseCharacter::getIsAlive()
{
    return isAlive;
}

void BaseCharacter::setIsAlive(bool isAlive)
{
    this->isAlive = isAlive;
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollisionRectangle()
{
    return Rectangle {
        getScreenPos().x,
        getScreenPos().y,
        scale * width,
        scale * height
    };
}

void BaseCharacter::tick(float deltaTime)
{
    // At first we save our current world position before changing anything so that
    // we can undo
    worldPosLastFrame = worldPos;

    // If any movement occurs, the length of the player velocity
    // vector will become non-zero
    if(Vector2Length(velocity) != 0.0f)
    {       
        // As we know, we're moving 1 unit every direction as the lengh of the
        // unit vectors of both x and y axis is 1 regardless of the direction. However
        // this is not the case when we move diagonally. Assuming we're moving diagonally
        // at both positive x and y axes, the length of our movement vector will be 
        // sqrt(1^2 + 1^2) = 1.4.... So, we will end up moving at a faster rate if we
        // move diagnally which we don't want. We want to move at the same rate
        // (1 unit in this case) in every direction. So we have to normalize
        // our movement vector while doing calculation

        // Also if we want to make the movement(velocityVector) faster or slower, we can just scale the
        // movment vector by a scalar

        // worldPos = worldPos + velocityVector
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));

        // Update the direction the character is facing
        rightLeft = (velocity.x < 0.0f) ? -1 : 1;

        // Update character texture state based on movement (idle or running)
        texture = runningTexture;
    }
    else
    {
        texture = idleTexture;
    }

    // Reset the velocity vector once the changes are acknowledged
    velocity = {};

    // Update character animation
    animRunningTime += deltaTime;
    if(animRunningTime >= animUpdateTime)
    {
        animRunningTime = 0.0f;

        animFrame++;
        // Zero based index so -1
        if(animFrame > (animMaxFrame - 1))
        {
            animFrame = 0;
        }
    }

    // Draw character texture
    Rectangle sourceRec
    {
        animFrame * width,
        0.0f,
        // Multipling the width with a negative value will flip the texture
        rightLeft * width,
        height
    };

    Rectangle destRec 
    {
        getScreenPos().x,
        getScreenPos().y,
        (scale * width),
        (scale * height)
    };

    // Draw texture based on the appropriate (idle/running) state
    // We're using a zero vector as the origin point
    DrawTexturePro(texture, sourceRec, destRec, Vector2{}, 0.0f, WHITE);
}