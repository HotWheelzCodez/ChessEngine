#include "../include/raylib.h"

#include "../include/board.hpp"

int main(void)
{
  constexpr int windowWidth = 800;
  constexpr int windowHeight = 800;

  InitWindow(windowWidth, windowHeight, "Chess Engine");
  SetTargetFPS(60);

  int monitor = GetCurrentMonitor();
  float monitorWidth = GetMonitorWidth(monitor);
  float monitorHeight = GetMonitorHeight(monitor);
  float windowScale = (float)monitorWidth/1000;

  float scaledWindowWidth = windowWidth*windowScale;
  float scaledWindowHeight = windowHeight*windowScale;

  SetTargetFPS(60);

  Board board = Board();

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground({ 28, 28, 35, 255 });

    board.UpdateAndRender({ 10, 10 });

    EndDrawing();
  }

  CloseWindow();
}
