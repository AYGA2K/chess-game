#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <iostream>
#include <string_view>

#include "board.h"
#include "constants.h"
#include "drawing.h"
#include "event_handling.h"
#include "texture_manager.h"
#include "types.h"

int main() {

  State *state = new State();

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_DisplayID display = SDL_GetPrimaryDisplay();
  if (!display) {
    std::cout << "No video display found: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display);
  if (!mode) {
    std::cout << "Failed to get display mode: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  state->windowWidth = mode->w;
  state->windowHeight = mode->h;

  SDL_CreateWindowAndRenderer("Chess Game", state->windowWidth,
                              state->windowHeight, SDL_WINDOW_RESIZABLE,
                              &state->Window, &state->Renderer);

  if (!state->Window || !state->Renderer) {
    std::cout << "SDL_CreateWindowAndRenderer failed: " << SDL_GetError()
              << std::endl;
    SDL_Quit();
    return 1;
  }

  if (!initializePieceTextures(state->Renderer)) {
    std::cout << "Failed to initialize piece textures: " << SDL_GetError()
              << std::endl;
    SDL_DestroyRenderer(state->Renderer);
    SDL_DestroyWindow(state->Window);
    SDL_Quit();
    return 1;
  }

  if (!initializeBoard(state)) {
    std::cout << "Failed to initialize board" << std::endl;
    cleanupPieceTextures();
    SDL_DestroyRenderer(state->Renderer);
    SDL_DestroyWindow(state->Window);
    SDL_Quit();
    return 1;
  }

  while (state->running) {
    SDL_Event Event;
    SDL_WaitEvent(&Event);
    state->running = HandleEvent(&Event, state);
    if (state->running) {
      SDL_SetRenderDrawColor(state->Renderer, BACKGROUND.r, BACKGROUND.g,
                             BACKGROUND.b, BACKGROUND.a);
      SDL_RenderClear(state->Renderer);
      drawBoard(state);
      state->running = SDL_RenderPresent(state->Renderer);
    }
  }

  std::string_view error(SDL_GetError());
  if (!error.empty()) {
    std::cout << "Error: " << error << std::endl;
  }

  cleanupBoard(state);
  cleanupPieceTextures();

  SDL_DestroyRenderer(state->Renderer);
  SDL_DestroyWindow(state->Window);
  SDL_Quit();

  delete state;

  return 0;
}
