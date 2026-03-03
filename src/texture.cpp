#include "constants.h"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <iostream>
#include <string>

SDL_Texture *loadTexture(const std::string &path, SDL_Renderer *renderer) {
  SDL_Texture *texture = nullptr;
  SDL_Surface *loadedSurface = SDL_LoadPNG(path.c_str());
  if (loadedSurface == nullptr) {
    std::cerr << "Unable to load image " << path
              << "! Error: " << SDL_GetError() << std::endl;
  } else {
    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr) {
      std::cerr << "Unable to create texture from " << path
                << "! Error: " << SDL_GetError() << std::endl;
    }
    SDL_DestroySurface(loadedSurface);
  }
  return texture;
}

bool renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, float x,
                   float y) {
  SDL_FRect destRect = {x, y, static_cast<float>(PIECE_SIZE),
                        static_cast<float>(PIECE_SIZE)};
  return SDL_RenderTexture(renderer, texture, nullptr, &destRect);
}
