#pragma once

#include "types.h"
#include <SDL3/SDL_render.h>

bool initFonts();
void cleanupFonts();

SDL_FRect getStartButtonRect(State *state);
SDL_FRect getQuitButtonRect(State *state);

bool drawStartScreen(State *state);
bool drawBoard(State *state);
bool drawSquare(SDL_Renderer *Renderer, SDL_Color color, int x, int y);
