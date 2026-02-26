#include "drawing.h"
#include "constants.h"
#include "types.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <iostream>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, SDL_Texture *> pieceTextures;

bool drawRectangle(SDL_Renderer *Renderer, SDL_Color color, int x, int y) {
  if (!SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a)) {
    return false;
  }
  SDL_FRect rect;
  rect.x = x;
  rect.y = y;
  rect.w = cellSize;
  rect.h = cellSize;

  if (!SDL_RenderFillRect(Renderer, &rect)) {
    return false;
  }
  return true;
}

bool drawBoard(State *state) {
  const int x_start = (state->windowWidth - cols * cellSize) / 2;
  const int y_start = (state->windowHeight - rows * cellSize) / 2;
  bool wasLight = false;

  for (int row = 0; row < rows; row++) {
    wasLight = !wasLight;
    for (int col = 0; col < cols; col++) {
      if (wasLight) {
        if (!drawRectangle(state->Renderer, LIGHT_SQUARE,
                           x_start + col * cellSize,
                           y_start + row * cellSize)) {
          return false;
        }
        wasLight = false;
      } else {
        if (!drawRectangle(state->Renderer, DARK_SQUARE,
                           x_start + col * cellSize,
                           y_start + row * cellSize)) {
          return false;
        }
        wasLight = true;
      }

      // Draw pieces on the board
      if (state->board[row][col].has_value()) {
        Piece *piece = state->board[row][col].value();
        if (piece) {
          // Calculate position (centered in the cell)
          float pieceX =
              x_start + col * cellSize + (cellSize - pieceSize) / 2.0f;
          float pieceY =
              y_start + row * cellSize + (cellSize - pieceSize) / 2.0f;

          SDL_FRect destRect = {pieceX, pieceY, static_cast<float>(pieceSize),
                                static_cast<float>(pieceSize)};

          if (!SDL_RenderTexture(state->Renderer, piece->texture, nullptr,
                                 &destRect)) {
            return false;
          }
        }
      }
    }
  }

  return true;
}

bool initializePieceTextures(SDL_Renderer *Renderer) {
  const char *pieceFiles[] = {
      "assets/brook.png",  "assets/bknight.png", "assets/bbishop.png",
      "assets/bqueen.png", "assets/bking.png",   "assets/bpawn.png",
      "assets/wrook.png",  "assets/wknight.png", "assets/wbishop.png",
      "assets/wqueen.png", "assets/wking.png",   "assets/wpawn.png"};

  for (const char *file : pieceFiles) {
    SDL_Surface *loadedSurface = SDL_LoadPNG(file);
    if (loadedSurface == nullptr) {
      std::cerr << "Unable to load image " << file
                << "! Error: " << SDL_GetError() << std::endl;
      cleanupPieceTextures();
      return false;
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(Renderer, loadedSurface);
    SDL_DestroySurface(loadedSurface);

    if (texture == nullptr) {
      std::cerr << "Unable to create texture from " << file
                << "! Error: " << SDL_GetError() << std::endl;
      cleanupPieceTextures();
      return false;
    }

    pieceTextures[file] = texture;
  }

  return true;
}

void cleanupPieceTextures() {
  for (auto &pair : pieceTextures) {
    if (pair.second) {
      SDL_DestroyTexture(pair.second);
    }
  }
  pieceTextures.clear();
}

Piece *getPiece(int row, int col) {
  // Row 0 and 1: Black pieces
  // Row 6 and 7: White pieces

  // Black pieces
  if (row == 0) {
    switch (col) {
    case 0:
    case 7: {
      Piece *piece = new Piece();
      piece->color = PieceColor::BLACK;
      piece->type = PieceType::ROOK;
      piece->texture = pieceTextures["assets/brook.png"];
      return piece;
    }
    case 1:
    case 6: {
      Piece *piece = new Piece();
      piece->color = PieceColor::BLACK;
      piece->type = PieceType::KNIGHT;
      piece->texture = pieceTextures["assets/bknight.png"];
      return piece;
    }
    case 2:
    case 5: {
      Piece *piece = new Piece();
      piece->color = PieceColor::BLACK;
      piece->type = PieceType::BISHOP;
      piece->texture = pieceTextures["assets/bbishop.png"];
      return piece;
    }
    case 3: {
      Piece *piece = new Piece();
      piece->color = PieceColor::BLACK;
      piece->type = PieceType::QUEEN;
      piece->texture = pieceTextures["assets/bqueen.png"];
      return piece;
    }
    case 4: {
      Piece *piece = new Piece();
      piece->color = PieceColor::BLACK;
      piece->type = PieceType::KING;
      piece->texture = pieceTextures["assets/bking.png"];
      return piece;
    }
    }
  } else if (row == 1) {
    // Black pawns
    Piece *piece = new Piece();
    piece->color = PieceColor::BLACK;
    piece->type = PieceType::PAWN;
    piece->texture = pieceTextures["assets/bpawn.png"];
    return piece;
  }

  // White pieces
  if (row == 6) {
    // White pawns
    Piece *piece = new Piece();
    piece->color = PieceColor::WHITE;
    piece->type = PieceType::PAWN;
    piece->texture = pieceTextures["assets/wpawn.png"];
    return piece;
  } else if (row == 7) {
    switch (col) {
    case 0:
    case 7: {
      Piece *piece = new Piece();
      piece->color = PieceColor::WHITE;
      piece->type = PieceType::ROOK;
      piece->texture = pieceTextures["assets/wrook.png"];
      return piece;
    }
    case 1:
    case 6: {
      Piece *piece = new Piece();
      piece->color = PieceColor::WHITE;
      piece->type = PieceType::KNIGHT;
      piece->texture = pieceTextures["assets/wknight.png"];
      return piece;
    }
    case 2:
    case 5: {
      Piece *piece = new Piece();
      piece->color = PieceColor::WHITE;
      piece->type = PieceType::BISHOP;
      piece->texture = pieceTextures["assets/wbishop.png"];
      return piece;
    }
    case 3: {
      Piece *piece = new Piece();
      piece->color = PieceColor::WHITE;
      piece->type = PieceType::QUEEN;
      piece->texture = pieceTextures["assets/wqueen.png"];
      return piece;
    }
    case 4: {
      Piece *piece = new Piece();
      piece->color = PieceColor::WHITE;
      piece->type = PieceType::KING;
      piece->texture = pieceTextures["assets/wking.png"];
      return piece;
    }
    }
  }

  return nullptr;
}

bool initializeBoard(State *state) {
  if (!state) {
    return false;
  }

  // Clear the board first
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      state->board[row][col] = nullptr;
    }
  }

  // Initialize black pieces (rows 0 and 1)
  for (int col = 0; col < 8; col++) {
    // Black pawns (row 1)
    Piece *blackPawn = new Piece();
    blackPawn->color = PieceColor::BLACK;
    blackPawn->type = PieceType::PAWN;
    blackPawn->texture = pieceTextures["assets/bpawn.png"];
    state->board[1][col] = blackPawn;
  }

  // Black back row (row 0)
  // Rooks
  for (int col : {0, 7}) {
    Piece *rook = new Piece();
    rook->color = PieceColor::BLACK;
    rook->type = PieceType::ROOK;
    rook->texture = pieceTextures["assets/brook.png"];
    state->board[0][col] = rook;
  }

  // Knights
  for (int col : {1, 6}) {
    Piece *knight = new Piece();
    knight->color = PieceColor::BLACK;
    knight->type = PieceType::KNIGHT;
    knight->texture = pieceTextures["assets/bknight.png"];
    state->board[0][col] = knight;
  }

  // Bishops
  for (int col : {2, 5}) {
    Piece *bishop = new Piece();
    bishop->color = PieceColor::BLACK;
    bishop->type = PieceType::BISHOP;
    bishop->texture = pieceTextures["assets/bbishop.png"];
    state->board[0][col] = bishop;
  }

  // Queen
  Piece *blackQueen = new Piece();
  blackQueen->color = PieceColor::BLACK;
  blackQueen->type = PieceType::QUEEN;
  blackQueen->texture = pieceTextures["assets/bqueen.png"];
  state->board[0][3] = blackQueen;

  // King
  Piece *blackKing = new Piece();
  blackKing->color = PieceColor::BLACK;
  blackKing->type = PieceType::KING;
  blackKing->texture = pieceTextures["assets/bking.png"];
  state->board[0][4] = blackKing;

  // Initialize white pieces (rows 6 and 7)
  for (int col = 0; col < 8; col++) {
    // White pawns (row 6)
    Piece *whitePawn = new Piece();
    whitePawn->color = PieceColor::WHITE;
    whitePawn->type = PieceType::PAWN;
    whitePawn->texture = pieceTextures["assets/wpawn.png"];
    state->board[6][col] = whitePawn;
  }

  // White back row (row 7)
  // Rooks
  for (int col : {0, 7}) {
    Piece *rook = new Piece();
    rook->color = PieceColor::WHITE;
    rook->type = PieceType::ROOK;
    rook->texture = pieceTextures["assets/wrook.png"];
    state->board[7][col] = rook;
  }

  // Knights
  for (int col : {1, 6}) {
    Piece *knight = new Piece();
    knight->color = PieceColor::WHITE;
    knight->type = PieceType::KNIGHT;
    knight->texture = pieceTextures["assets/wknight.png"];
    state->board[7][col] = knight;
  }

  // Bishops
  for (int col : {2, 5}) {
    Piece *bishop = new Piece();
    bishop->color = PieceColor::WHITE;
    bishop->type = PieceType::BISHOP;
    bishop->texture = pieceTextures["assets/wbishop.png"];
    state->board[7][col] = bishop;
  }

  // Queen
  Piece *whiteQueen = new Piece();
  whiteQueen->color = PieceColor::WHITE;
  whiteQueen->type = PieceType::QUEEN;
  whiteQueen->texture = pieceTextures["assets/wqueen.png"];
  state->board[7][3] = whiteQueen;

  // King
  Piece *whiteKing = new Piece();
  whiteKing->color = PieceColor::WHITE;
  whiteKing->type = PieceType::KING;
  whiteKing->texture = pieceTextures["assets/wking.png"];
  state->board[7][4] = whiteKing;

  return true;
}

void cleanupBoard(State *state) {
  if (!state) {
    return;
  }

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (state->board[row][col].has_value()) {
        Piece *piece = state->board[row][col].value();
        if (piece) {
          delete piece;
        }
        state->board[row][col] = nullptr;
      }
    }
  }
}
