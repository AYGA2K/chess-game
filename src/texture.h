#pragma once
#include <SDL3/SDL_render.h>
#include <string>

SDL_Texture *loadTexture(const std::string &path, SDL_Renderer *renderer);
bool renderTexture(SDL_Texture *loadedTexture, SDL_Renderer *renderer, float x,
                   float y);
