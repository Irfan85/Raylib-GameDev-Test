#include "BaseCharacter.h"
#include "Character.h"

#include "raylib.h"

class Enemy : public BaseCharacter
{
    public:
        Enemy(Vector2 pos, Texture2D idleTexture, Texture2D runningTexture, int animMaxFrame);
        void setTarget(Character *target);
        virtual Vector2 getScreenPos() override;
        virtual void tick(float deltaTime) override;
    private:
        Character *target {nullptr};
        const float damagePerSecond {10.0f};
        // The enemy will stop chasing and will go idle state if it reaches within
        // "chaseRaduis" amount of pixels from the player's screenPos
        const float chaseRadius {25.0f};
};