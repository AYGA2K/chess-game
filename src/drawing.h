#pragma once

#include "types.h"
#include <SDL3/SDL_render.h>

bool drawBoard(State *state);

bool initializePieceTextures(SDL_Renderer *Renderer);

void cleanupPieceTextures();

Piece *getPiece(int row, int col);

bool initializeBoard(State *state);
void cleanupBoard(State *state);
