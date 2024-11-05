#pragma once

#include <stdint.h>

#include "../raylib.h"

namespace Gui 
{
  enum Flags 
  {
    FLAG_KEEP_SQUARE = 0b00000001
  };

  enum TextAlignments
  {
    TEXT_ALIGNMENT_LEFT   = 0,
    TEXT_ALIGNMENT_CENTER = 1,
    TEXT_ALIGNMENT_RIGHT  = 2
  };

  typedef struct {
    Color baseBackgroundColor;
    Color baseTextColor;
    Color baseOutlineColor;
    Color hoverBackgroundColor;
    Color hoverTextColor;
    Color hoverOutlineColor;
    Font  font;
    int   fontSize;
    int   textAligmnt;
    float roundness;
    float outlineThickness;
  } ButtonStyle;

  bool Button(Rectangle bounds, ButtonStyle style, const char* text, unsigned int flag);
  bool Button(Rectangle bounds, ButtonStyle style, void (*drawFunc)(int x, int y, int width, int height, Gui::ButtonStyle style), unsigned int flag);
}
