#pragma once

#include "types.h"

bool validateMove(State *state, int toRow, int toCol);
bool IsEmpty(State *state, int row, int col);
bool HasOpponentPiece(State *state, int row, int col, PieceColor pieceColor);
