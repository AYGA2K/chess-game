#include "game_logic.h"
#include "constants.h"
#include <iostream>

bool IsEmpty(State *state, int row, int col) {
  if (!state->board[row][col].has_value()) {
    return true;
  }
  return state->board[row][col].value() == nullptr;
}

bool HasOpponentPiece(State *state, int row, int col, PieceColor pieceColor) {
  if (!state->board[row][col].has_value()) {
    return false;
  }

  Piece *piece = state->board[row][col].value();
  if (piece == nullptr) {
    return false;
  }

  return piece->color != pieceColor;
}

bool validateMove(State *state, int toRow, int toCol) {
  std::cout << "validateMove" << std::endl;
  std::cout << "toRow: " << toRow << std::endl;
  std::cout << "toCol: " << toCol << std::endl;
  std::cout << "selectedRow: " << state->selectedRow << std::endl;
  std::cout << "selectedCol: " << state->selectedCol << std::endl;
  Piece *piece = state->board[state->selectedRow][state->selectedCol].value();
  if (!piece) {
    return false;
  }

  const int selectedRow = state->selectedRow;
  const int selectedCol = state->selectedCol;
  const int dr = toRow - selectedRow;
  const int dc = toCol - selectedCol;

  // Can't move to the same square
  if (dr == 0 && dc == 0) {
    return false;
  }

  switch (piece->type) {
  case PieceType::PAWN: {
    const int direction = (piece->color == PieceColor::BLACK) ? 1 : -1;
    const int startRow = (piece->color == PieceColor::BLACK) ? 1 : 6;
    bool isEmptyTo = IsEmpty(state, toRow, toCol);
    bool isEmptyMid = IsEmpty(state, selectedRow + direction, selectedCol);

    // Double move
    if (dc == 0 && dr == 2 * direction && selectedRow == startRow &&
        isEmptyTo && isEmptyMid) {
      return true;
    }

    // Single move
    if (dc == 0 && dr == direction && isEmptyTo) {
      return true;
    }

    // Diagonal attack
    if (dr == direction && std::abs(dc) == 1 &&
        HasOpponentPiece(state, toRow, toCol, piece->color)) {
      return true;
    }
    return false;
  }
  case PieceType::KNIGHT: {
    if ((std::abs(dr) == 2 && std::abs(dc) == 1) ||
        (std::abs(dr) == 1 && std::abs(dc) == 2)) {
      return true;
    }
    return false;
  }
  case PieceType::BISHOP:
    if (std::abs(dr) == std::abs(dc)) {
      // Check if the diagonal path is clear
      int rowStep = (dr > 0) ? 1 : -1;
      int colStep = (dc > 0) ? 1 : -1;
      int checkRow = selectedRow + rowStep;
      int checkCol = selectedCol + colStep;

      while (checkRow != toRow && checkCol != toCol) {
        if (!IsEmpty(state, checkRow, checkCol)) {
          return false;
        }
        checkRow += rowStep;
        checkCol += colStep;
      }

      // Destination must be empty or have opponent piece
      if (IsEmpty(state, toRow, toCol) ||
          HasOpponentPiece(state, toRow, toCol, piece->color)) {
        return true;
      }
    }
    return false;
  case PieceType::ROOK:
    if (dr == 0 || dc == 0) {
      // Check if the path is clear
      if (dr == 0) {
        // Moving horizontally
        int step = (dc > 0) ? 1 : -1;
        for (int col = selectedCol + step; col != toCol; col += step) {
          if (!IsEmpty(state, selectedRow, col)) {
            return false;
          }
        }
      } else {
        // Moving vertically
        int step = (dr > 0) ? 1 : -1;
        for (int row = selectedRow + step; row != toRow; row += step) {
          if (!IsEmpty(state, row, selectedCol)) {
            return false;
          }
        }
      }

      // Destination must be empty or have opponent piece
      if (IsEmpty(state, toRow, toCol) ||
          HasOpponentPiece(state, toRow, toCol, piece->color)) {
        return true;
      }
    }
    return false;
  case PieceType::QUEEN:
    if (dr == 0 || dc == 0 || std::abs(dr) == std::abs(dc)) {
      // Check if the path is clear
      if (dr == 0) {
        // Moving horizontally
        int step = (dc > 0) ? 1 : -1;
        for (int col = selectedCol + step; col != toCol; col += step) {
          if (!IsEmpty(state, selectedRow, col)) {
            return false;
          }
        }
      } else if (dc == 0) {
        // Moving vertically
        int step = (dr > 0) ? 1 : -1;
        for (int row = selectedRow + step; row != toRow; row += step) {
          if (!IsEmpty(state, row, selectedCol)) {
            return false;
          }
        }
      } else {
        // Moving diagonally
        int rowStep = (dr > 0) ? 1 : -1;
        int colStep = (dc > 0) ? 1 : -1;
        int checkRow = selectedRow + rowStep;
        int checkCol = selectedCol + colStep;

        while (checkRow != toRow && checkCol != toCol) {
          if (!IsEmpty(state, checkRow, checkCol)) {
            return false;
          }
          checkRow += rowStep;
          checkCol += colStep;
        }
      }

      // Destination must be empty or have opponent piece
      if (IsEmpty(state, toRow, toCol) ||
          HasOpponentPiece(state, toRow, toCol, piece->color)) {
        return true;
      }
    }
    return false;
  case PieceType::KING:
    if (std::abs(dr) <= 1 && std::abs(dc) <= 1) {
      // Destination must be empty or have opponent piece
      if (IsEmpty(state, toRow, toCol) ||
          HasOpponentPiece(state, toRow, toCol, piece->color)) {
        return true;
      }
    }
    return false;
  default:
    return false;
  }
}
