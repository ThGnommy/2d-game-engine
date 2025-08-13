#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

class AssetStore {

public:
  AssetStore();
  ~AssetStore();

  AssetStore(const AssetStore&) = delete;
  AssetStore operator=(const AssetStore&) = delete;
  AssetStore(AssetStore&&) = delete;
  AssetStore& operator=(AssetStore&&) = delete;

  void ClearAssets();
  void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);
  SDL_Texture* GetTexture(const std::string& assetId) const;

private:
  std::unordered_map<std::string, SDL_Texture*> texturesMap{};

};

#endif
