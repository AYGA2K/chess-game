#pragma once
#include <SDL3/SDL_render.h>
#include <array>
#include <optional>

enum class PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum class PieceColor { WHITE, BLACK };

struct Piece {
  PieceType type;
  PieceColor color;
  SDL_Texture *texture;
};

struct State {
  bool running = true;
  SDL_Renderer *Renderer;
  SDL_Window *Window;
  int windowWidth;
  int windowHeight;
  std::array<std::array<std::optional<Piece *>, 8>, 8> board;
  int selectedRow;
  int selectedCol;
  bool pieceSelected;
  PieceColor currentPlayer = PieceColor::WHITE;
};
