#pragma once /* BOARD_HPP */

#include "../include/board.hpp"

Board::Board(void)
  : m_Board(std::move(std::vector<char>(64))), m_PeicesTextures(std::move(std::vector<Rectangle>(12))), m_Peices(std::move(std::vector<std::pair<char, char>>(24)))
{
  // Load the different peice textures
  m_TextureAtlas = LoadTexture("assets/textures/chessPeices.png");
  m_PeicesTextures[WHITE_PAWN]   = { (5*m_PeiceWidth), 0, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[BLACK_PAWN]   = { (5*m_PeiceWidth), m_PeiceWidth, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[WHITE_KNIGHT] = { (3*m_PeiceWidth), 0, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[BLACK_KNIGHT] = { (3*m_PeiceWidth), m_PeiceWidth, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[WHITE_BISHOP] = { (2*m_PeiceWidth), 0, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[BLACK_BISHOP] = { (2*m_PeiceWidth), m_PeiceWidth, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[WHITE_ROOK]   = { (4*m_PeiceWidth), 0, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[BLACK_ROOK]   = { (4*m_PeiceWidth), m_PeiceWidth, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[WHITE_QUEEN]  = { m_PeiceWidth, 0, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[BLACK_QUEEN]  = { m_PeiceWidth, m_PeiceWidth, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[WHITE_KING]   = { 0, 0, m_PeiceWidth, m_PeiceWidth };
  m_PeicesTextures[BLACK_KING]   = { 0, m_PeiceWidth, m_PeiceWidth, m_PeiceWidth };

  // Setup the board with the starting position
  m_Peices = GeneratePeicesFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

// Destructor to unload any and all resources
Board::~Board(void)
{
  UnloadTexture(m_TextureAtlas);
}

// Will draw the board, the peices, and allow the user and computer to move the peices
void Board::UpdateAndRender(Vector2 boardPosition) noexcept
{
  float peiceScale = m_SquareWidth/m_PeiceWidth;

  // Draw board
  for (char i = 0; i < 8; i++)
  {
    for (char j = 0; j < 8; j++)
    {
      bool isDarkSquare = (i + j) % 2 != 0; // Determine square color
      Color squareColor = WHITE;
      if (isDarkSquare)
          squareColor = BROWN;

      // Draw the square
      DrawRectangle(boardPosition.x+(j*m_SquareWidth), boardPosition.y+(i*m_SquareWidth), m_SquareWidth, m_SquareWidth, squareColor);
    }
  }

  // Draw peices
  for (char i = 0; i < m_Peices.size(); i++)
  {
    std::pair<char, char> peice = m_Peices[i];
    
    char rank = peice.second/8;
    char file = peice.second%8;

    Rectangle dest = { boardPosition.x+(file*m_SquareWidth), boardPosition.y+(rank*m_SquareWidth), m_PeiceWidth*peiceScale, m_PeiceWidth*peiceScale };
    DrawTexturePro(m_TextureAtlas, m_PeicesTextures[peice.first], dest, { 0, 0 }, 0.0f, WHITE);
  }
}

std::vector<std::pair<char, char>> Board::GeneratePeicesFromFen(const std::string_view fen) noexcept
{
  std::vector<std::pair<char, char>> peices;

  char rank = 0;
  char file = 0;
  for (const char& c : fen)
  {
    if (c == '/') 
    {
      rank++;
      file = 0;
      continue;
    }

    if (isdigit(c))
    {
      file += c-'0';
      continue;
    }

    char position = (rank*8)+file;

    if (isupper(c))
    {
      switch (c)
      {
        case 'P':
          peices.push_back(std::pair<char, char>(WHITE_PAWN, position));
          break;
        case 'B':
          peices.push_back(std::pair<char, char>(WHITE_BISHOP, position));
          break;
        case 'N':
          peices.push_back(std::pair<char, char>(WHITE_KNIGHT, position));
          break;
        case 'R':
          peices.push_back(std::pair<char, char>(WHITE_ROOK, position));
          break;
        case 'Q':
          peices.push_back(std::pair<char, char>(WHITE_QUEEN, position));
          break;
        case 'K':
          peices.push_back(std::pair<char, char>(WHITE_KING, position));
          break;
      }
    }
    else
    {
      switch (c)
      {
        case 'p':
          peices.push_back(std::pair<char, char>(BLACK_PAWN, position));
          break;
        case 'b':
          peices.push_back(std::pair<char, char>(BLACK_BISHOP, position));
          break;
        case 'n':
          peices.push_back(std::pair<char, char>(BLACK_KNIGHT, position));
          break;
        case 'r':
          peices.push_back(std::pair<char, char>(BLACK_ROOK, position));
          break;
        case 'q':
          peices.push_back(std::pair<char, char>(BLACK_QUEEN, position));
          break;
        case 'k':
          peices.push_back(std::pair<char, char>(BLACK_KING, position));
          break;
      }
    }

    file++;
  }

  return peices;
}
