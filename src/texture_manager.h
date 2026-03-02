#pragma once

#include <SDL3/SDL_render.h>
#include <string>
#include <unordered_map>

bool initializePieceTextures(SDL_Renderer *Renderer);
void cleanupPieceTextures();
SDL_Texture *getTexture(const std::string &textureName);
