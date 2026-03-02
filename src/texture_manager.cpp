#include "texture_manager.h"
#include <SDL3/SDL_surface.h>
#include <iostream>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, SDL_Texture *> pieceTextures;

bool initializePieceTextures(SDL_Renderer *Renderer) {
  const char *pieceFiles[] = {
      "assets/brook.png",  "assets/bknight.png", "assets/bbishop.png",
      "assets/bqueen.png", "assets/bking.png",   "assets/bpawn.png",
      "assets/wrook.png",  "assets/wknight.png", "assets/wbishop.png",
      "assets/wqueen.png", "assets/wking.png",   "assets/wpawn.png"};

  for (const char *file : pieceFiles) {
    SDL_Surface *loadedSurface = SDL_LoadPNG(file);
    if (loadedSurface == nullptr) {
      std::cerr << "Unable to load image " << file
                << "! Error: " << SDL_GetError() << std::endl;
      cleanupPieceTextures();
      return false;
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(Renderer, loadedSurface);
    SDL_DestroySurface(loadedSurface);

    if (texture == nullptr) {
      std::cerr << "Unable to create texture from " << file
                << "! Error: " << SDL_GetError() << std::endl;
      cleanupPieceTextures();
      return false;
    }

    pieceTextures[file] = texture;
  }

  return true;
}

void cleanupPieceTextures() {
  for (auto &pair : pieceTextures) {
    if (pair.second) {
      SDL_DestroyTexture(pair.second);
    }
  }
  pieceTextures.clear();
}

SDL_Texture *getTexture(const std::string &textureName) {
  auto it = pieceTextures.find(textureName);
  if (it != pieceTextures.end()) {
    return it->second;
  }
  return nullptr;
}
