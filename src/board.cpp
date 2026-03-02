#include "board.h"
#include "texture_manager.h"

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
    blackPawn->texture = getTexture("assets/bpawn.png");
    state->board[1][col] = blackPawn;
  }

  // Black back row (row 0)
  // Rooks
  for (int col : {0, 7}) {
    Piece *rook = new Piece();
    rook->color = PieceColor::BLACK;
    rook->type = PieceType::ROOK;
    rook->texture = getTexture("assets/brook.png");
    state->board[0][col] = rook;
  }

  // Knights
  for (int col : {1, 6}) {
    Piece *knight = new Piece();
    knight->color = PieceColor::BLACK;
    knight->type = PieceType::KNIGHT;
    knight->texture = getTexture("assets/bknight.png");
    state->board[0][col] = knight;
  }

  // Bishops
  for (int col : {2, 5}) {
    Piece *bishop = new Piece();
    bishop->color = PieceColor::BLACK;
    bishop->type = PieceType::BISHOP;
    bishop->texture = getTexture("assets/bbishop.png");
    state->board[0][col] = bishop;
  }

  // Queen
  Piece *blackQueen = new Piece();
  blackQueen->color = PieceColor::BLACK;
  blackQueen->type = PieceType::QUEEN;
  blackQueen->texture = getTexture("assets/bqueen.png");
  state->board[0][3] = blackQueen;

  // King
  Piece *blackKing = new Piece();
  blackKing->color = PieceColor::BLACK;
  blackKing->type = PieceType::KING;
  blackKing->texture = getTexture("assets/bking.png");
  state->board[0][4] = blackKing;

  // Initialize white pieces (rows 6 and 7)
  for (int col = 0; col < 8; col++) {
    // White pawns (row 6)
    Piece *whitePawn = new Piece();
    whitePawn->color = PieceColor::WHITE;
    whitePawn->type = PieceType::PAWN;
    whitePawn->texture = getTexture("assets/wpawn.png");
    state->board[6][col] = whitePawn;
  }

  // White back row (row 7)
  // Rooks
  for (int col : {0, 7}) {
    Piece *rook = new Piece();
    rook->color = PieceColor::WHITE;
    rook->type = PieceType::ROOK;
    rook->texture = getTexture("assets/wrook.png");
    state->board[7][col] = rook;
  }

  // Knights
  for (int col : {1, 6}) {
    Piece *knight = new Piece();
    knight->color = PieceColor::WHITE;
    knight->type = PieceType::KNIGHT;
    knight->texture = getTexture("assets/wknight.png");
    state->board[7][col] = knight;
  }

  // Bishops
  for (int col : {2, 5}) {
    Piece *bishop = new Piece();
    bishop->color = PieceColor::WHITE;
    bishop->type = PieceType::BISHOP;
    bishop->texture = getTexture("assets/wbishop.png");
    state->board[7][col] = bishop;
  }

  // Queen
  Piece *whiteQueen = new Piece();
  whiteQueen->color = PieceColor::WHITE;
  whiteQueen->type = PieceType::QUEEN;
  whiteQueen->texture = getTexture("assets/wqueen.png");
  state->board[7][3] = whiteQueen;

  // King
  Piece *whiteKing = new Piece();
  whiteKing->color = PieceColor::WHITE;
  whiteKing->type = PieceType::KING;
  whiteKing->texture = getTexture("assets/wking.png");
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
