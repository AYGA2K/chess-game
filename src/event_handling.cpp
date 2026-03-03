#include "event_handling.h"
#include "board.h"
#include "constants.h"
#include "drawing.h"
#include "game_logic.h"
#include "types.h"
#include <iostream>

bool HandleEvent(SDL_Event *Event, State *state) {
  switch (Event->type) {
  case SDL_EVENT_QUIT:
    std::cout << "Quit event received" << std::endl;
    return false;
    break;
  case SDL_EVENT_WINDOW_RESIZED:
    state->windowWidth = Event->window.data1;
    state->windowHeight = Event->window.data2;
    return true;
  case SDL_EVENT_MOUSE_BUTTON_DOWN: {
    if (Event->button.button == SDL_BUTTON_LEFT) {
      if (state->screen == GameScreen::START) {
        float mx = Event->button.x;
        float my = Event->button.y;
        SDL_FRect startRect = getStartButtonRect(state);
        SDL_FRect quitRect = getQuitButtonRect(state);
        if (mx >= startRect.x && mx <= startRect.x + startRect.w &&
            my >= startRect.y && my <= startRect.y + startRect.h) {
          state->screen = GameScreen::GAME;
        } else if (mx >= quitRect.x && mx <= quitRect.x + quitRect.w &&
                   my >= quitRect.y && my <= quitRect.y + quitRect.h) {
          return false;
        }
        return true;
      }

      int mouseX = Event->button.x;
      int mouseY = Event->button.y;

      int x_start = (state->windowWidth - BOARD_SIZE * CELL_SIZE) / 2;
      int y_start = (state->windowHeight - BOARD_SIZE * CELL_SIZE) / 2;
      if (mouseX < x_start || mouseY < y_start) {
        if (state->pieceSelected) {
          state->pieceSelected = false;
        }
        return true;
      }

      int col = (mouseX - x_start) / CELL_SIZE;
      int row = (mouseY - y_start) / CELL_SIZE;

      if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return true;

      if (!state->pieceSelected) {
        if (state->board[row][col] != nullptr) {
          Piece *piece = state->board[row][col].value();
          if (piece && piece->color == state->currentPlayer) {
            state->pieceSelected = true;
            state->selectedRow = row;
            state->selectedCol = col;
          }
        }
      } else {
        int fromRow = state->selectedRow;
        int fromCol = state->selectedCol;

        if (!validateMove(state, row, col)) {
          state->pieceSelected = false;
          return true;
        }

        // Check if capturing a king
        if (state->board[row][col].has_value()) {
          Piece *capturedPiece = state->board[row][col].value();
          if (capturedPiece && capturedPiece->type == PieceType::KING) {
            std::cout << (state->currentPlayer == PieceColor::WHITE ? "White"
                                                                    : "Black")
                      << " wins!" << std::endl;
            cleanupBoard(state);
            initializeBoard(state);
            state->currentPlayer = PieceColor::WHITE;
            state->pieceSelected = false;
            state->screen = GameScreen::START;
            return true;
          }
        }

        state->board[row][col] = state->board[fromRow][fromCol];

        state->board[fromRow][fromCol] = std::nullopt;

        state->pieceSelected = false;

        // Switch turns
        state->currentPlayer = (state->currentPlayer == PieceColor::WHITE)
                                   ? PieceColor::BLACK
                                   : PieceColor::WHITE;
      }
    }
    return true;
  } break;
  default:
    return true;
  }
}
