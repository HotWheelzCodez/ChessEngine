#pragma once /* BOARD_HPP */

#include <iostream>
#include <vector>
#include <string>

#include "raylib.h"

enum Peices
{
  WHITE_PAWN = 0,
  WHITE_BISHOP,
  WHITE_KNIGHT,
  WHITE_ROOK,
  WHITE_QUEEN,
  WHITE_KING,
  BLACK_PAWN,
  BLACK_BISHOP,
  BLACK_KNIGHT,
  BLACK_ROOK,
  BLACK_QUEEN,
  BLACK_KING
};

typedef struct Peice
{
  char type;
  char boardIndex;
  bool selected;
  bool moved;
} Peice;

static bool mousePressed = false;

class Board
{
public:
  Board(void);
  ~Board(void);

  void UpdateAndRender(Vector2 boardPosition) noexcept;

private:
  std::vector<char> m_Board;
  std::vector<Rectangle> m_PeicesTextures;
  std::vector<Peice> m_Peices; // The peice type and the peice index (relative to the board)
  Texture2D m_TextureAtlas;
  static constexpr float m_SquareWidth = 75;
  static constexpr float m_PeiceWidth = 45.0f;

private:
  [[nodiscard]] std::vector<Peice> GeneratePeicesFromFen(const std::string& filePath) noexcept;
  void GetLegalMoves(Peice& peice) noexcept;
};
