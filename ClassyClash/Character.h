#ifndef CHARACTER_H
#define CHARACTER_H

#include "BaseCharacter.h"

#include "raylib.h"

class Character : public BaseCharacter
{
    public:
        Character(int windowWidth, int windowHeight, 
        Texture2D idleTexture, Texture2D runningTexture, int animMaxFrame);
        // Just using 'const' for demonstration
        Rectangle getSwordCollisionRec() const;
        float getHealth() const;
        void takeDamage(float damage);
        virtual Vector2 getScreenPos() override;
        virtual void tick(float deltaTime) override;

    private:
        float windowWidth {};
        float windowHeight {};
        float health {100.0f};

        Texture2D swordTexture {LoadTexture("characters/weapon_sword.png")};
        // As we have shifter the swords origin in the tick() function, 
        // we have to define the bounding box of the sword ourselves
        Rectangle swordCollisionRec {};
};

#endif