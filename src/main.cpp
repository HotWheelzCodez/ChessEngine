#include <iostream>

#include "../include/raylib.h"

int main(void)
{
  InitWindow(800, 800, "Ray Chess");
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  CloseWindow();
}
