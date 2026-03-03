#include "drawing.h"
#include "constants.h"
#include "types.h"
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

static TTF_Font *s_titleFont = nullptr;
static TTF_Font *s_buttonFont = nullptr;

bool initFonts() {
  if (!TTF_Init()) {
    return false;
  }
  s_titleFont = TTF_OpenFont("assets/fonts/DejaVuSans-Bold.ttf", 72.0f);
  s_buttonFont = TTF_OpenFont("assets/fonts/DejaVuSans.ttf", 28.0f);
  return s_titleFont && s_buttonFont;
}

void cleanupFonts() {
  if (s_titleFont) {
    TTF_CloseFont(s_titleFont);
    s_titleFont = nullptr;
  }
  if (s_buttonFont) {
    TTF_CloseFont(s_buttonFont);
    s_buttonFont = nullptr;
  }
  TTF_Quit();
}

static bool renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                       SDL_Color color, float x, float y) {
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, color);
  if (!surface) {
    return false;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);
  if (!texture) {
    return false;
  }

  float w, h;
  SDL_GetTextureSize(texture, &w, &h);
  SDL_FRect dst = {x - w / 2.0f, y - h / 2.0f, w, h};
  SDL_RenderTexture(renderer, texture, nullptr, &dst);
  SDL_DestroyTexture(texture);
  return true;
}

SDL_FRect getStartButtonRect(State *state) {
  return {(state->windowWidth - BUTTON_WIDTH) / 2.0f,
          state->windowHeight / 2.0f, static_cast<float>(BUTTON_WIDTH),
          static_cast<float>(BUTTON_HEIGHT)};
}

SDL_FRect getQuitButtonRect(State *state) {
  return {(state->windowWidth - BUTTON_WIDTH) / 2.0f,
          state->windowHeight / 2.0f + BUTTON_HEIGHT + BUTTON_GAP,
          static_cast<float>(BUTTON_WIDTH), static_cast<float>(BUTTON_HEIGHT)};
}

bool renderButton(SDL_Renderer *renderer, SDL_Color fillColor,
                  SDL_Color borderColor, const char *text, SDL_FRect rect) {
  SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b,
                         fillColor.a);
  SDL_RenderFillRect(renderer, &rect);
  SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b,
                         borderColor.a);
  SDL_RenderRect(renderer, &rect);

  SDL_Color white = {255, 255, 255, 255};
  renderText(renderer, s_buttonFont, text, white, rect.x + rect.w / 2.0f,
             rect.y + rect.h / 2.0f);
  return true;
}

bool drawStartScreen(State *state) {
  const float cx = state->windowWidth / 2.0f;

  renderText(state->Renderer, s_titleFont, "Chess", {255, 255, 255, 255}, cx,
             state->windowHeight * 0.28f);

  SDL_FRect startRect = getStartButtonRect(state);
  SDL_FRect quitRect = getQuitButtonRect(state);

  if (!renderButton(state->Renderer, BTN_START_FILL, BTN_START_BORDER, "Start",
                    startRect)) {
    return false;
  }
  if (!renderButton(state->Renderer, BTN_QUIT_FILL, BTN_QUIT_BORDER, "Quit",
                    quitRect)) {
    return false;
  }
  return true;
}

bool drawSquare(SDL_Renderer *Renderer, SDL_Color color, int x, int y) {
  if (!SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a)) {
    return false;
  }
  SDL_FRect rect;
  rect.x = x;
  rect.y = y;
  rect.w = CELL_SIZE;
  rect.h = CELL_SIZE;

  if (!SDL_RenderFillRect(Renderer, &rect)) {
    return false;
  }
  return true;
}

bool drawBoard(State *state) {
  const int x_start = (state->windowWidth - BOARD_SIZE * CELL_SIZE) / 2;
  const int y_start = (state->windowHeight - BOARD_SIZE * CELL_SIZE) / 2;
  bool wasLight = false;

  // Draw turn indicator
  SDL_Color turnColor = (state->currentPlayer == PieceColor::WHITE)
                            ? SDL_Color{255, 255, 255, 255} // White
                            : SDL_Color{0, 0, 0, 255};      // Black

  // Draw indicator on the left side of the board
  int indicatorX = x_start - 120;
  int indicatorY = y_start + (BOARD_SIZE * CELL_SIZE) / 2 - 30;
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

  for (int row = 0; row < BOARD_SIZE; row++) {
    wasLight = !wasLight;
    for (int col = 0; col < BOARD_SIZE; col++) {
      if (wasLight) {
        if (!drawSquare(state->Renderer, LIGHT_SQUARE,
                        x_start + col * CELL_SIZE, y_start + row * CELL_SIZE)) {
          return false;
        }
        wasLight = false;
      } else {
        if (!drawSquare(state->Renderer, DARK_SQUARE, x_start + col * CELL_SIZE,
                        y_start + row * CELL_SIZE)) {
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
              x_start + col * CELL_SIZE + (CELL_SIZE - PIECE_SIZE) / 2.0f;
          float pieceY =
              y_start + row * CELL_SIZE + (CELL_SIZE - PIECE_SIZE) / 2.0f;

          SDL_FRect destRect = {pieceX, pieceY, static_cast<float>(PIECE_SIZE),
                                static_cast<float>(PIECE_SIZE)};

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
