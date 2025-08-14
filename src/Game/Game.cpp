#include "Game.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../Logger/Logger.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "SDL2/SDL_render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

Game::Game() {
  isRunning = false;
  assetStore = std::make_unique<AssetStore>();
  Logger::Log("Game constructor called!");
}

Game::~Game() { Logger::Log("Game destructor called!"); }

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    Logger::Err("Error initializing SDL.");
    return;
  }

  // setup w and h
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = displayMode.w;
  windowHeight = displayMode.h;

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1024, 768, SDL_WINDOW_ALWAYS_ON_TOP);

  if (!window) {
    Logger::Err("Error creating SDL window.");
    return;
  }

  // Create a 2D rendering context for a window.
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    Logger::Err("Error creating SDL renderer.");
    return;
  }

  // Change the video mode to REAL fullscreen
  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;

  // Poll the events
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
        isRunning = false;
      }
      break;
    }
  }
}

void Game::LoadLevel(const int level) {
  // Add systems
  EntityManager::Get().AddSystem<MovementSystem>();
  EntityManager::Get().AddSystem<RenderSystem>();

  // temp added texture
  assetStore->AddTexture(renderer, "tank-panther-down",
                         "./assets/images/tank-panther-down.png");

  Entity tank = EntityManager::Get().CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(100.0, 100.0),
                                        glm::vec2(3.0, 3.0), 45.0);
  tank.AddComponent<RigidbodyComponent>(glm::vec2(10.0, 10.0));
  tank.AddComponent<SpriteComponent>("tank-panther-down", 32, 32);

  // TODO: Load the tilemap
  assetStore->AddTexture(renderer, "jungle", "./assets/tilemaps/jungle.png");

  std::vector<std::vector<int>> tileMatrix{};

  std::string line;
  std::ifstream mapFile;
  mapFile.open("./assets/tilemaps/jungle.map");

  int index = 0;

  if (mapFile.is_open()) {
    while (getline(mapFile, line)) {

      std::string tile{};

      tileMatrix.emplace_back(std::vector<int>{});

      for (int i = 0; i < line.size(); i++) {
        if (!std::isdigit(line[i])) {
          tileMatrix[index].emplace_back(std::stoi(tile));
          tile = "";
          continue;
        }

        tile += line[i];
      }

      index++;
    }

    mapFile.close();
  }

  // Actually draw the tilemap
  constexpr int tileSize{32};
  constexpr double tileScale{2.0};

  SDL_Surface *surface = IMG_Load("./assets/tilemaps/jungle.png");
  const int tileWidth = surface->w;
  SDL_FreeSurface(surface);

  for (int i{0}; i < tileMatrix.size(); i++) {
    for (int j{0}; j < tileMatrix[i].size(); j++) {

      // How many row the tilemap image has
      const int tilesPerRow = tileWidth / tileSize;

      // The index of the current tile
      const int tileIndex = tileMatrix[i][j];

      // Find what row in the tilemap I have to look
      const int row = floor(tileIndex / tilesPerRow);

      const int srcX = (tileIndex - (row * tilesPerRow)) * tileSize;
      const int srcY = row * tileSize;

      Entity tile = EntityManager::Get().CreateEntity();

      tile.AddComponent<TransformComponent>(
          glm::vec2(((tileSize * tileScale) * j), ((tileSize * tileScale) * i)),
          glm::vec2(tileScale, tileScale));
      tile.AddComponent<SpriteComponent>("jungle", tileSize, tileSize, srcX,
                                         srcY);
    }
  }
}

void Game::Setup() { LoadLevel(1); }

void Game::Update() {
  // If we are too fast, waste some time until we reach MILLISECS_PER_FRAME
  const unsigned int timeToWait =
      MILLISECS_PER_FRAME - (SDL_GetTicks64() - millisecsPrevFrame);

  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  const double deltaTime = (SDL_GetTicks64() - millisecsPrevFrame) / 1000.0;

  millisecsPrevFrame = SDL_GetTicks64();

  // TODO: update game objects...

  // Ask all the systems to update
  _getEntityManager().GetSystem<MovementSystem>().Update(deltaTime);

  // Update the entity manager to process the entities that are waiting to be
  // created/deleted
  _getEntityManager().Update();
}

void Game::Render() {

  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  _getEntityManager().GetSystem<RenderSystem>().Update(renderer, assetStore);

  SDL_RenderPresent(renderer);
}

void Game::Run() {

  Setup();

  // Game loop...
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

EntityManager &Game::_getEntityManager() { return EntityManager::Get(); }
