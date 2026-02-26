#pragma once

#include <SDL3/SDL_pixels.h>

// Board dimensions
inline constexpr int cols = 8;
inline constexpr int rows = 8;
inline constexpr int cellSize = 80;
inline constexpr int pieceSize = 70;

// Chessboard colors
inline constexpr SDL_Color LIGHT_SQUARE{245, 230, 196, 255};
inline constexpr SDL_Color DARK_SQUARE{166, 120, 82, 255};

// Background / Panels
inline constexpr SDL_Color BACKGROUND{29, 31, 38, 255};
inline constexpr SDL_Color INFO_PANEL{27, 29, 35, 255};

// Text / Highlights
inline constexpr SDL_Color TEXT_COLOR{255, 255, 255, 255};
inline constexpr SDL_Color STAR_COLOR{255, 215, 0, 255};
inline constexpr SDL_Color TIMER_GREEN{76, 175, 80, 255};

// Buttons
inline constexpr SDL_Color RESIGN_BUTTON{211, 47, 47, 255};
inline constexpr SDL_Color PLAY_BUTTON{66, 66, 66, 255};
