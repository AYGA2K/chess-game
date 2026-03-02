#include "drawing.h"
#include "constants.h"
#include "types.h"
#include <SDL3/SDL_render.h>

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

  // Draw turn indicator
  SDL_Color turnColor = (state->currentPlayer == PieceColor::WHITE)
                            ? SDL_Color{255, 255, 255, 255} // White
                            : SDL_Color{0, 0, 0, 255};      // Black

  // Draw indicator on the left side of the board
  int indicatorX = x_start - 60;
  int indicatorY = y_start + (rows * cellSize) / 2 - 30;
  int indicatorSize = 60;

  SDL_SetRenderDrawColor(state->Renderer, turnColor.r, turnColor.g, turnColor.b,
                         turnColor.a);
  SDL_FRect turnIndicator = {
      static_cast<float>(indicatorX), static_cast<float>(indicatorY),
      static_cast<float>(indicatorSize), static_cast<float>(indicatorSize)};
  SDL_RenderFillRect(state->Renderer, &turnIndicator);

  // Draw border around turn indicator
  SDL_SetRenderDrawColor(state->Renderer, 200, 200, 200, 255);
  SDL_RenderRect(state->Renderer, &turnIndicator);

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
