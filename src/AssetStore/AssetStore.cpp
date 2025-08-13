#include "AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

AssetStore::AssetStore() { Logger::Log("AssetStore constructor called!"); }

AssetStore::~AssetStore() {
  ClearAssets();
  Logger::Log("AssetStore destructor called!");
}

void AssetStore::ClearAssets() {
  for (auto texture : texturesMap) {
    SDL_DestroyTexture(texture.second);
  }

  texturesMap.clear();
}

void AssetStore::AddTexture(SDL_Renderer *renderer, const std::string &assetId,
                            const std::string &path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  texturesMap.emplace(assetId, texture);

  Logger::Log("AssetStore: Texture added to the AssetStore with id: " + assetId + " and path " + path);
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId) const {
  return texturesMap.at(assetId);
}
