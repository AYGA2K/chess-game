#pragma once

#include <SDL3/SDL_pixels.h>

// Board dimensions
inline constexpr int BOARD_SIZE = 8;
inline constexpr int CELL_SIZE = 80;
inline constexpr int PIECE_SIZE = 70;

// Start screen buttons
inline constexpr int BUTTON_WIDTH = 220;
inline constexpr int BUTTON_HEIGHT = 60;
inline constexpr int BUTTON_GAP = 20;

// Button colors
inline constexpr SDL_Color BTN_START_FILL{56, 142, 242, 255};
inline constexpr SDL_Color BTN_START_BORDER{100, 175, 255, 255};
inline constexpr SDL_Color BTN_QUIT_FILL{210, 60, 60, 255};
inline constexpr SDL_Color BTN_QUIT_BORDER{240, 100, 100, 255};

// Chessboard colors
inline constexpr SDL_Color LIGHT_SQUARE{245, 230, 196, 255};
inline constexpr SDL_Color DARK_SQUARE{166, 120, 82, 255};

// Background / Panels
inline constexpr SDL_Color BACKGROUND{29, 31, 38, 255};
