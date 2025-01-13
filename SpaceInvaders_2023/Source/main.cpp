#include "game.h"

#define screenWidth  1920
#define screenHeight  1080

int main(void)
{   

    InitWindow(screenWidth, screenHeight, "SPACE INVADERS");
    SetTargetFPS(60);
    Game game = { State::STARTSCREEN };

    while (!WindowShouldClose())   
    {
        game.Update();
      
        BeginDrawing();
        ClearBackground(BLACK);
        game.Render();
        EndDrawing();
    }
    CloseWindow();       
    return 0;
}