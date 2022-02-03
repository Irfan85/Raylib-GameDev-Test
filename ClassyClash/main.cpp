#include <string>

#include "raylib.h"
#include "raymath.h"

#include "Character.h"
#include "Prop.h"
#include "Enemy.h"

int main ()
{
    // ***** Window properties *****
    const int windowWidth {384};
    const int windowHeight {384};
    InitWindow(windowWidth, windowHeight, "Classy Clash");

    // ***** Game properties *****
    SetTargetFPS(60);

    // ***** Map texture *****
    Texture2D mapTexture {LoadTexture("nature_tileset/OpenWorldMap24x24.png")};
    Vector2 mapTexturePos {0.0f, 0.0f};
    const float mapTextureScale {4.0f};

    // ***** Setting up characters *****
    Character knight {
        windowWidth, 
        windowHeight,
        LoadTexture("characters/knight_idle_spritesheet.png"),
        LoadTexture("characters/knight_run_spritesheet.png"),
        6
    };

    // ***** Setting up enemies *****
    Enemy enemy1 {
        Vector2 {800.0f, 300.0f},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png"),
        6
    };
    enemy1.setTarget(&knight);

    Enemy enemy2 {
        Vector2 {500.0f, 700.0f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png"),
        6
    };
    enemy2.setTarget(&knight);

    Enemy *enemies[] {&enemy1, &enemy2};

    // ***** Setting up props *****
    Prop props[2] {
        Prop {Vector2 {600.0f, 300.0f}, Texture2D {LoadTexture("nature_tileset/Rock.png")}},
        Prop {Vector2 {400.0f, 500.0f}, Texture2D {LoadTexture("nature_tileset/Log.png")}}
    };

    // ***** Game loop *****
    while(!WindowShouldClose())
    {   
        BeginDrawing();
        ClearBackground(WHITE);
    
        // The player will be stationary and the map will move instead. So the 
        // movement of the map has to be in the opposite direction of the player
        // in order to create the illusion of the player's movment. So, we have to scale
        // the world position vector of the character by -1 to negate that. This will be our map
        // position vecotor and thus the map will move in the opposite direction of the 
        // player movement
        mapTexturePos = Vector2Scale(knight.getWorldPos(), -1.0f);

        // Draw map texture
        DrawTextureEx(mapTexture, mapTexturePos, 0.0f, mapTextureScale, WHITE);

        // Draw prop textures
        for(auto prop : props)
        {
            prop.render(knight.getWorldPos());
        }

        // No need to draw anything if the knight is dead
        if(!knight.getIsAlive())
        {
            DrawText("Game Over!", 55.0f, 45.0f, 40, RED);

            EndDrawing();
            // Ignore the rest of the loop body
            continue;
        }
        else
        {
            // If the knight is alive, show health status text
            std::string knightHealthStr {"Health: "};
            // Append till 4th character of the health floating point. This will show till 2 digits
            // after the decimal point
            knightHealthStr.append(std::to_string(knight.getHealth()), 0, 5);

            DrawText(knightHealthStr.c_str(), 55.0f, 45.0f, 40, RED);
        }

        // Draw the characters
        knight.tick(GetFrameTime());

        // Check whether the player is escaping map bounds
        if(knight.getWorldPos().x < 0.0f
            || knight.getWorldPos().y < 0.0f
            || (knight.getWorldPos().x + windowWidth) > (mapTextureScale * mapTexture.width)
            || (knight.getWorldPos().y + windowHeight) > (mapTextureScale * mapTexture.height))
        {
            // If the player tries to escape the boundary, undo that movement to get
            // back within valid map range
            knight.undoMovement();
        }

        // Check whether the player is colliding with any of the props. If any collision is detected
        // undo the player's movement if the player tries to get through the prop
        for(auto prop : props)
        {
            if(CheckCollisionRecs(knight.getCollisionRectangle(), prop.getCollisionRectangle(knight.getWorldPos())))
            {
                knight.undoMovement();
            }
        }

        for(auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        // Check for collisions between the player's weapon and the enemies
        // Notice we're checking for mouse 'pressed' because we don't want the player to spam input
        // by holding the mouse button down continiously
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if(CheckCollisionRecs(knight.getSwordCollisionRec(), enemy1.getCollisionRectangle()))
            {
                // If an enemy collides with the sword, destroy it
                for(auto enemy : enemies)
                {
                    enemy->setIsAlive(false);
                }
            }
        }
 
        EndDrawing();
    }

    UnloadTexture(mapTexture);

    CloseWindow();

    return 0;
}