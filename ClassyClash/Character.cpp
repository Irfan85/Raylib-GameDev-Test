#include "Character.h"

#include "raymath.h"

Character::Character(int windowWidth, int windowHeight,
                    Texture2D idleTexture, Texture2D runningTexture, int animMaxFrame) 
: BaseCharacter{idleTexture, runningTexture, animMaxFrame},
windowWidth(windowWidth), windowHeight(windowHeight)
{
    speed = 4.0f;
}

// Screenpos will always be the constant for the player
// We want the character to stay at the center of the window
Vector2 Character::getScreenPos()
{
    return Vector2 {
        (float)windowWidth/2.0f - 0.5f * (scale * width),
        (float)windowHeight/2.0f - 0.5f * (scale * height)
    };
}

Rectangle Character::getSwordCollisionRec() const
{
    return swordCollisionRec;
}

float Character::getHealth() const
{
    return health;
}
void Character::takeDamage(float damage)
{
    health -= damage;

    if(health <= 0.0f)
    {
        setIsAlive(false);
    }
}

void Character::tick(float deltaTime)
{
    // If the character is dead, do nothing and exit out of the function
    if(!getIsAlive())
    {
        return;
    }

    // Update the velocity vector based on player's movement
    if(IsKeyDown(KEY_A))
    {
        velocity.x -= 1.0f;
    }
    if(IsKeyDown(KEY_D))
    {
        velocity.x += 1.0f;
    }
    if(IsKeyDown(KEY_W))
    {
        velocity.y -= 1.0f;
    }
    if(IsKeyDown(KEY_S))
    {
        velocity.y += 1.0f;
    }

    // Call the parent class tick function that does the animation and drawing stuff
    BaseCharacter::tick(deltaTime);

    // Draw the sword

    // We want the origin of the sword to be in the bottom left postion when the player
    // is facing right. Similarly, we want it to be in the bottom right position when the
    // player is moving left
    Vector2 swordOrigin {};

    // This is to adjust the postion of the sword. We will hardcode this based on trial and error
    // whichever value looks good
    Vector2 swordOffset {};

    // Rotation of the sword will also change depending on which direction the chracter is facing
    // when the user clicks left mouse button for attack animation
    float swordRotation {};
    
    if(rightLeft > 0)
    {
        swordOrigin = {
            0.0f,
            scale * swordTexture.height
        };

        swordOffset = {25.0f, 55.0f};

        swordCollisionRec = {
            getScreenPos().x + swordOffset.x,
            getScreenPos().y + swordOffset.y - (swordTexture.height * scale),
            (swordTexture.width * scale),
            (swordTexture.height * scale)
        };

        IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? swordRotation = 35.0f : 0;
    }
    else
    {
        swordOrigin = {
            scale * swordTexture.width,
            scale * swordTexture.height
        };
        
        swordOffset = {35.0f, 55.0f};

        swordCollisionRec = {
            getScreenPos().x + swordOffset.x - (swordTexture.width * scale),
            getScreenPos().y + swordOffset.y - (swordTexture.height * scale),
            (swordTexture.width * scale),
            (swordTexture.height * scale)
        };

        IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? swordRotation = -35.0f : 0;
    }

    // Here, we're using C++'s own casting system instead of C style casting. It ensures compile time safely
    Rectangle swordSourceRec {0.0f, 0.0f, rightLeft * static_cast<float>(swordTexture.width), static_cast<float>(swordTexture.height)};
    Rectangle swordDestRec {(getScreenPos().x + swordOffset.x), (getScreenPos().y + swordOffset.y), scale * swordTexture.width, scale * swordTexture.height};

    DrawTexturePro(swordTexture, swordSourceRec, swordDestRec, swordOrigin, swordRotation, WHITE);

    // [DEBUG PURPOSE] Draw sword collision line
    // DrawRectangleLines(swordCollisionRec.x, swordCollisionRec.y, swordCollisionRec.width, swordCollisionRec.height, GREEN);
}

