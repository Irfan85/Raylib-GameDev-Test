#include "Enemy.h"

#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D idleTexture, Texture2D runningTexture, int animMaxFrame)
: BaseCharacter{idleTexture, runningTexture, animMaxFrame}
{
    worldPos = pos;
    speed = 3.5f;
}

void Enemy::setTarget(Character *target)
{
   this->target = target;
}

// screenPos = worldPos - playersWorldPos. This will give us a position vector
// with respect to the window's top left(0, 0) point as oppose to the map's top-left
// point which the worldPos represents. That way we will be able to know where we
// should draw the enemy
Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::tick(float deltaTime)
{
    // If the character is dead, do nothing and exit out of the function
    if(!getIsAlive())
    {
        return;
    }

    // Calculate the movement(velocity vector) so that the enemy chases the player
    // At first we need to find out the vector from enemy to taget
    // velocityVector = positionVectorOfDestination - positionVectorOfSource
    // This formula is achieved by applying vector addition/triangle rule
    // As in this game the screen position of the player is stationary, we want 
    // the enemies to run toward the center.
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if(Vector2Length(velocity) < chaseRadius)
    {
        velocity = {};
    }

    // Do drawing and animation stuff
    BaseCharacter::tick(deltaTime);

    // Check for collision with the player. If collision occurs conduct damage
    if(CheckCollisionRecs(getCollisionRectangle(), target->getCollisionRectangle()))
    {
        target->takeDamage(damagePerSecond * GetFrameTime());
    }
}