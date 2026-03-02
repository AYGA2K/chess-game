#pragma once

#include "types.h"
#include <SDL3/SDL_render.h>

bool drawBoard(State *state);
bool drawRectangle(SDL_Renderer *Renderer, SDL_Color color, int x, int y);
