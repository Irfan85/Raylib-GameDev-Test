#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

#include "raylib.h"

class BaseCharacter
{
    public:
        // Since the player character and enemy characters will have different movement, they will
        // be positioned differently (player locked to center and enemies free to move), we're not
        // taking any position vectors in the base class constructor. They will be set up in their
        // respective classes accordingly
        BaseCharacter(Texture2D idleTexture, 
                        Texture2D runningTexture, int animMaxFrame);
        Vector2 getWorldPos();
        // The screenPos says where to draw the characater on the window or screen
        // This will be stationary for the player and will be at the center of the window. However, for any other
        // character it will be used to move that character to the opposite direction of the player to create the
        // illusion of the player's movement. As the player moves around, the characters may appear in the screen or may
        // get out of the screen's view. This vector is used for those calculaitons. As screenPos needs to be calculated
        // frequently and is dependent on worldPos, there is no need for making an additional variable for it. So,
        // we'll access it using this method.

        // "= 0" means it's a pure virtual function that will not have any implementation in the base class
        // by adding a pure virtual function in the base class, we're also making the base class abstract so
        // it can't be initialized by itself.
        virtual Vector2 getScreenPos() = 0;
        bool getIsAlive();
        void setIsAlive(bool isAlive);
        Rectangle getCollisionRectangle();
        void undoMovement();
        // This function will do necessary update to the character such as position,
        // animation etc in each frame
        virtual void tick(float deltaTime);
    protected:
        // Texture to use
        Texture2D texture {};
        // Texture based on different states of the character
        Texture2D idleTexture {};
        Texture2D runningTexture {};
        
        // The worldPos says where the character is located in the world with respect to the top left corner(0, 0) of
        // the maps texture. We use this to put any object in a certain position in the map.
        Vector2 worldPos {};
        // This will help use to backtrack movement if character goes out of the map bound and undo
        // that move
        Vector2 worldPosLastFrame {};
        Vector2 velocity {};
        float width {};
        float height {};
        const float scale {4.0f};
        // Determines whether the character is facing right or left
        // 1 for right, -1 for left
        int rightLeft {1};
        float speed {};    

        // Animation properties
        int animFrame {};
        int animMaxFrame {};
        float animRunningTime {};
        const float animUpdateTime {1.0f/12.0f};
        
    private:
        bool isAlive {true};
};

#endif