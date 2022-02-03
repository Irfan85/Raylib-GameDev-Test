#include "raylib.h"

int main()
{
    // Game window initialization
    int windowWidth {800};
    int windowHeight {450};
    InitWindow(windowWidth, windowHeight, "Axe Game");

    // Game properties
    SetTargetFPS(60);
    bool collisionWithAxe {false};

    // Circle properties
    int circleX {200};
    int circleY {200};
    int circleRadius {25};
    // Circle Edges
    int circleLeftX;
    int circleRightX;
    int circleTopY;
    int circleBottomY;

    // Axe properties
    int axeX {400};
    int axeY {0};
    int axeLength {50};
    int axeDirection {2};
    // Axe Edges
    int axeLeftX;
    int axeRightX;
    int axetopY;
    int axeBottomY;

    // Game loop
    while(!WindowShouldClose())
    {
        BeginDrawing(); // Uses double buffering
        ClearBackground(WHITE);

        if(collisionWithAxe)
        {
            DrawText("Game Over!", 400, 200, 20, RED);
        }
        else
        {
            // Game logic beigns

            // Move the axe
            axeY += axeDirection;
            if(axeY < 0 || axeY > windowHeight)
            {
                axeDirection = -(axeDirection);
            }

            // Moving circle to right
            if(IsKeyDown(KEY_D) && circleX < windowWidth)
            {
                circleX += 10;
            }

            // Moving circle to left
            if(IsKeyDown(KEY_A) && circleX > 0)
            {
                circleX -= 10;
            }

            // Calculating circle edges
            circleLeftX = circleX - circleRadius;
            circleRightX = circleX + circleRadius;
            circleTopY = circleY - circleRadius;
            circleBottomY = circleY + circleRadius;
            // Calculating axe edges
            axeLeftX = axeX;
            axeRightX = axeX + axeLength;
            axetopY = axeY;
            axeBottomY = axeY + axeLength;

            // Check for collision and update collision status
            collisionWithAxe = (axeBottomY >= circleTopY)
                                && (axetopY <= circleBottomY)
                                && (axeLeftX <= circleRightX)
                                && (axeRightX >= circleLeftX);

            // Drawing objects
            DrawCircle(circleX, circleY, circleRadius, BLUE);
            DrawRectangle(axeX, axeY, axeLength, axeLength, RED);
            
            // Game logic ends
        }
        
        EndDrawing();
    }
    
    return 0;
}