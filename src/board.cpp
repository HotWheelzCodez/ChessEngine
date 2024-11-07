#include "../include/board.hpp"

Board::Board(void)
  : m_Board(std::move(std::vector<char>(64))), m_PeicesTextures(std::move(std::vector<Rectangle>(12))), m_Peices(std::move(std::vector<Peice>(24)))
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
  const std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
  m_Peices = GeneratePeicesFromFen(fen);
}

// Destructor to unload any and all resources
Board::~Board(void)
{
  UnloadTexture(m_TextureAtlas);
}

// Will draw the board, the peices, and allow the user and computer to move the peices
void Board::UpdateAndRender(Vector2 boardPosition) noexcept
{
  if (IsKeyPressed(KEY_ESCAPE))
    for (Peice& peice : m_Peices)
      peice.selected = false;

  mousePressed = false;
  
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
    Peice& peice = m_Peices[i];

    char rank = peice.boardIndex/8;
    char file = peice.boardIndex%8;
    char index = (rank*8)+file;

    // Get the legal moves for the given peice
    GetLegalMoves(peice);

    

    Rectangle dest = { boardPosition.x+(file*m_SquareWidth), boardPosition.y+(rank*m_SquareWidth), m_PeiceWidth*peiceScale, m_PeiceWidth*peiceScale };

    // Check if the user clicked the peice
    Vector2 mousePos = GetMousePosition();
    mousePos.x += boardPosition.x;
    mousePos.y += boardPosition.y;
   
    if (mousePos.x > m_SquareWidth*8 || mousePos.y > m_SquareWidth*8)
    {
      DrawTexturePro(m_TextureAtlas, m_PeicesTextures[peice.type], dest, { 0, 0 }, 0.0f, WHITE);
      continue;
    }

    Color backgroundColor = { 0, 0, 0, 0 };
    mousePos.x /= m_SquareWidth;
    mousePos.y /= m_SquareWidth;

    char selectedRank = mousePos.y;
    char selectedFile = mousePos.x;
    char selectedIndex = (selectedRank*8)+selectedFile;

    if (selectedIndex == index)
    {
      backgroundColor = BEIGE;

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      {
        mousePressed = true;

        // Unselect all other peices
        for (Peice& tempPeice : m_Peices)
          tempPeice.selected = false;
        peice.selected = true;
      }

      
    }

    if (peice.selected)
    {
      backgroundColor = { 0, 0, 255, 100 };

      if (!mousePressed && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      {
        peice.boardIndex = selectedIndex;
        peice.selected = false;
        peice.moved = true;
      }

      // Show the legal moves
      for (const char& move : peice.legalIndices)
      {
        char moveRank = move/8;
        char moveFile = move%8;

        float x = moveFile*m_SquareWidth;
        float y = moveRank*m_SquareWidth;

        DrawRectangle(x, y, m_SquareWidth, m_SquareWidth, { 255, 0, 0, 100 });
      }
    }

    DrawRectangle(boardPosition.x+(file*m_SquareWidth), boardPosition.y+(rank*m_SquareWidth), m_SquareWidth, m_SquareWidth, backgroundColor);
    DrawTexturePro(m_TextureAtlas, m_PeicesTextures[peice.type], dest, { 0, 0 }, 0.0f, WHITE);
  }
}

void Board::GetLegalMoves(Peice& peice) noexcept
{
  peice.legalIndices.clear();
  switch (peice.type)
  {
    case WHITE_PAWN:
      if (peice.moved && peice.boardIndex > 8)
      {
        bool valid = true;
        for (Peice tempPeice : m_Peices)
        {
          if (tempPeice.boardIndex == peice.boardIndex-8)
            valid = false;

          if (tempPeice.boardIndex == peice.boardIndex-9 && tempPeice.type > WHITE_KING)
            peice.legalIndices.push_back(tempPeice.boardIndex);
          if (tempPeice.boardIndex == peice.boardIndex-7 && tempPeice.type > WHITE_KING)
            peice.legalIndices.push_back(tempPeice.boardIndex);
        }

        if (valid)
          peice.legalIndices.push_back(peice.boardIndex-8);
      }
      else if (peice.boardIndex > 16)
      {
        bool valid = true;
        for (Peice tempPeice : m_Peices)
        {
          if (tempPeice.boardIndex == peice.boardIndex-8)
            valid = false;
        }

        if (valid)
        {
          peice.legalIndices.push_back(peice.boardIndex-8);
          peice.legalIndices.push_back(peice.boardIndex-16);
        }
      }



      break;
    case WHITE_BISHOP:
      
      break;
  }
}

std::vector<Peice> Board::GeneratePeicesFromFen(const std::string& fen) noexcept
{
  std::vector<Peice> peices;

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
          peices.push_back({ WHITE_PAWN, position, false });
          break;
        case 'B':
          peices.push_back({ WHITE_BISHOP, position, false });
          break;
        case 'N':
          peices.push_back({ WHITE_KNIGHT, position, false });
          break;
        case 'R':
          peices.push_back({ WHITE_ROOK, position, false });
          break;
        case 'Q':
          peices.push_back({ WHITE_QUEEN, position, false });
          break;
        case 'K':
          peices.push_back({ WHITE_KING, position, false });
          break;
      }
    }
    else
    {
      switch (c)
      {
        case 'p':
          peices.push_back({ BLACK_PAWN, position, false });
          break;
        case 'b':
          peices.push_back({ BLACK_BISHOP, position, false });
          break;
        case 'n':
          peices.push_back({ BLACK_KNIGHT, position, false });
          break;
        case 'r':
          peices.push_back({ BLACK_ROOK, position, false });
          break;
        case 'q':
          peices.push_back({ BLACK_QUEEN, position, false });
          break;
        case 'k':
          peices.push_back({ BLACK_KING, position, false });
          break;
      }
    }

    file++;
  }

  return peices;
}
