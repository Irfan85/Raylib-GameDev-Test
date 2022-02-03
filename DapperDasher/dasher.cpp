#include "raylib.h"

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnAir(const AnimData &animData, const int windowHeight)
{
    return animData.pos.y < (windowHeight - animData.rect.height);
}

void updateAnimation(AnimData &animData, const float deltaTime, const int maxAnimFrameCount)
{
    animData.runningTime += deltaTime;

    // Update the animation when it's the proper time
    if (animData.runningTime >= animData.updateTime)
    {
        animData.rect.x = (animData.frame * animData.rect.width);

        animData.frame++;

        if (animData.frame > maxAnimFrameCount)
        {
            animData.frame = 0;
        }

        // Reset the running time when animation has ben updated
        animData.runningTime = 0.0f;
    }
}

int main()
{
    // ***** Window properties *****
    const int windowWidth{800};
    const int windowHeight{450};
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    // ***** Game properties *****
    SetTargetFPS(60);
    const int gravity{1'200}; // in pixels/second
    // Check for collision with nebula
    bool collisionDetected{false};
    bool hasWon {false};

    // ***** Scarfy texture properties *****
    Texture2D scarfy{LoadTexture("textures/scarfy.png")};

    AnimData scarfyAnimData;

    scarfyAnimData.rect.width = scarfy.width / 6;
    scarfyAnimData.rect.height = scarfy.height;
    scarfyAnimData.rect.x = 0;
    scarfyAnimData.rect.y = 0;

    scarfyAnimData.pos.x = (windowWidth / 2) - (scarfyAnimData.rect.width / 2);
    scarfyAnimData.pos.y = windowHeight - scarfyAnimData.rect.height;

    scarfyAnimData.frame = 0;
    // We want to update the animation 12 times per second
    scarfyAnimData.updateTime = (1.0f / 12.0f);
    // Time eclapsed since last animation update
    scarfyAnimData.runningTime = 0.0f;

    int scarfyJumpVelocity{0};
    const int scarfyJumpVelocityRate{-600};
    bool scarfyIsInAir{false};
    const int scarfyAnimMaxFrameCount = 5; // 0 to 5

    // ***** Nebula texture properties *****
    Texture2D nebula{LoadTexture("textures/12_nebula_spritesheet.png")};

    const int nebulaCount{16};
    const int nebulaAnimMaxFrameCount = 7; // 0 to 7

    AnimData nebulaAnimData[nebulaCount]{};

    for (int i = 0; i < nebulaCount; i++)
    {
        nebulaAnimData[i].rect.x = 0.0f;
        nebulaAnimData[i].rect.y = 0.0f;
        nebulaAnimData[i].rect.width = (nebula.width / 8.0f);
        nebulaAnimData[i].rect.height = (nebula.height / 8.0f);

        // We want to spawn the nebula outside the screen right after its
        // rightmost edge
        nebulaAnimData[i].pos.x = windowWidth + (700 * i);
        nebulaAnimData[i].pos.y = (windowHeight - (nebula.height / 8.0f));

        nebulaAnimData[i].frame = 0;
        nebulaAnimData[i].updateTime = (1.0f / 25.0f); // Will update 25 times per second
        nebulaAnimData[i].runningTime = 0.0f;
    }

    int nebulaVelocity{-300};

    // The finish line. Just like the nebulae, it will move towards the player
    // from right. We're initializing it as the x position of the last nebula
    float finishLine{nebulaAnimData[nebulaCount - 1].pos.x};

    // *** Background texture properties *****
    Texture2D backgroundTexture{LoadTexture("textures/far-buildings.png")};
    const float bgScale{4.0f};
    const int bgScrollRate{-50};
    float bgX{0.0f};

    Texture2D midgroundTexture{LoadTexture("textures/back-buildings.png")};
    const int mgScrollRate{-100};
    float mgX{0.0f};

    Texture2D foregroundTexture{LoadTexture("textures/foreground.png")};
    const int fgScrollRate{-150};
    float fgX{0.0f};

    // ***** Game loop *****
    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        // Draw background texture
        bgX += (bgScrollRate * deltaTime);
        if (bgX <= -(backgroundTexture.width * bgScale))
        {
            bgX = 0.0f;
        }

        Vector2 bg1Pos{bgX, 0.0f};
        DrawTextureEx(backgroundTexture, bg1Pos, 0.0f, bgScale, WHITE);

        Vector2 bg2Pos{bg1Pos.x + (backgroundTexture.width * bgScale), 0.0f};
        DrawTextureEx(backgroundTexture, bg2Pos, 0.0f, bgScale, WHITE);

        // Draw midground texture
        mgX += (mgScrollRate * deltaTime);
        if (mgX <= -(midgroundTexture.width * bgScale))
        {
            mgX = 0.0f;
        }

        Vector2 mg1Pos{mgX, 0.0f};
        DrawTextureEx(midgroundTexture, mg1Pos, 0.0f, bgScale, WHITE);

        Vector2 mg2Pos{mg1Pos.x + (midgroundTexture.width * bgScale), 0.0f};
        DrawTextureEx(midgroundTexture, mg2Pos, 0.0f, bgScale, WHITE);

        // Draw foreground texture
        fgX += (fgScrollRate * deltaTime);
        if (fgX <= -(foregroundTexture.width * bgScale))
        {
            fgX = 0.0f;
        }

        Vector2 fg1Pos{fgX, 0.0f};
        DrawTextureEx(foregroundTexture, fg1Pos, 0.0f, bgScale, WHITE);

        Vector2 fg2Pos{fg1Pos.x + (foregroundTexture.width * bgScale), 0.0f};
        DrawTextureEx(foregroundTexture, fg2Pos, 0.0f, bgScale, WHITE);

        // Object updating and drawing starts here. We won't do that if any
        // collision is detected (since game will be over) or the player has won.
        if(collisionDetected)
        {
            DrawText("GAME OVER!", (windowWidth/2) - 80, (windowHeight/2) - 20, 32, RED);
        }else if(hasWon)
        {
            DrawText("YOU WIN!", (windowWidth/2) - 80, (windowHeight/2) - 20, 32, GREEN);
        }
        else
        {
            // Check whether scarfy is on air
            if (isOnAir(scarfyAnimData, windowHeight))
            {
                // Applying gravity if the rectangle is above ground
                scarfyJumpVelocity += (gravity * deltaTime);
                scarfyIsInAir = true;
            }
            else
            {
                scarfyJumpVelocity = 0;
                scarfyIsInAir = false;
            }

            // Scarfy will jump when spacebar is pressed
            if (IsKeyDown(KEY_SPACE))
            {
                if (scarfyAnimData.pos.y > 0 && (!scarfyIsInAir))
                {
                    scarfyJumpVelocity += scarfyJumpVelocityRate;
                }
            }

            // Update scarfy position (jump)
            scarfyAnimData.pos.y += (scarfyJumpVelocity * deltaTime);
            if (scarfyAnimData.pos.y < 0)
            {
                scarfyAnimData.pos.y = 0;
            }

            // Don't animate when he is in the air
            if (!scarfyIsInAir)
            {
                updateAnimation(scarfyAnimData, deltaTime, scarfyAnimMaxFrameCount);
            }

            for (int i = 0; i < nebulaCount; i++)
            {
                // Update nebula animation
                updateAnimation(nebulaAnimData[i], deltaTime, nebulaAnimMaxFrameCount);

                // Update nebula position
                nebulaAnimData[i].pos.x += (nebulaVelocity * deltaTime);
            }

            for (AnimData nebulaData : nebulaAnimData)
            {
                // The rectangle in the AnimData represents the location of the
                // rectangle in the spritesheet. Not on the screen. So we have to create
                // new rectagnes with proper positions

                // Also there are some transparent padding around the nebula sprites
                // in the spritesheet. We won't want them since they will give incorrect
                // collision result. So, we will squeeze our new rectangle 'inside'.
                // So, we have to increase both x and y posiiton and decrease width
                // and height
                const int nebulaPadding{50};

                Rectangle nebulaRec{
                    nebulaData.pos.x + nebulaPadding,
                    nebulaData.pos.y + nebulaPadding,
                    nebulaData.rect.width - (2 * nebulaPadding),
                    nebulaData.rect.height - (2 * nebulaPadding)};

                // The scarfy spritesheet has very minimal padding so we can ignore that
                Rectangle scarfyRec{
                    scarfyAnimData.pos.x,
                    scarfyAnimData.pos.y,
                    scarfyAnimData.rect.width,
                    scarfyAnimData.rect.height};

                collisionDetected = CheckCollisionRecs(nebulaRec, scarfyRec);

                if(collisionDetected)
                {
                    break;
                }
            }

            // Update finish line position
            finishLine += (nebulaVelocity * deltaTime);
            if(scarfyAnimData.pos.x >= finishLine)
            {
                hasWon = true;
            }

            // Draw nebulae
            for (AnimData nebulaData : nebulaAnimData)
            {
                DrawTextureRec(nebula, nebulaData.rect, nebulaData.pos, WHITE);
            }

            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyAnimData.rect, scarfyAnimData.pos, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(backgroundTexture);
    UnloadTexture(midgroundTexture);
    UnloadTexture(foregroundTexture);

    CloseWindow();
}