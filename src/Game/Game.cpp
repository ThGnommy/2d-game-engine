#include "Game.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidbodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../Logger/Logger.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderDebugSystem.h"
#include "../Systems/RenderSystem.h"
#include "SDL2/SDL_render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

Game::Game() {
  _isRunning = false;
  _debugMode = false;
  _assetStore = std::make_unique<AssetStore>();
  _eventBus = std::make_unique<EventBus>();
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
  WindowWidth = displayMode.w;
  WindowHeight = displayMode.h;

  _window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WindowWidth, WindowHeight, SDL_WINDOW_BORDERLESS);

  if (!_window) {
    Logger::Err("Error creating SDL window.");
    return;
  }

  // Create a 2D rendering context for a window.
  _renderer = SDL_CreateRenderer(
      _window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!_renderer) {
    Logger::Err("Error creating SDL renderer.");
    return;
  }

  _initializeCameraView();

  // Change the video mode to REAL fullscreen
  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  _isRunning = true;
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;

  // Poll the events
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_QUIT:
      _isRunning = false;
      break;
    case SDL_KEYDOWN: {
      _eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
        _isRunning = false;
      }
    }
      if (sdlEvent.key.keysym.sym == SDLK_d) {
        _debugMode = !_debugMode;
      }
      break;
    }
  }
}

void Game::LoadLevel(const int level) {
  // Add systems
  EntityManager::Get().AddSystem<MovementSystem>();
  EntityManager::Get().AddSystem<RenderSystem>();
  EntityManager::Get().AddSystem<RenderDebugSystem>();
  EntityManager::Get().AddSystem<AnimationSystem>();
  EntityManager::Get().AddSystem<CollisionSystem>();
  EntityManager::Get().AddSystem<DamageSystem>();
  EntityManager::Get().AddSystem<KeyboardControlSystem>();
  EntityManager::Get().AddSystem<CameraMovementSystem>();

  // temp added texture
  _assetStore->AddTexture(_renderer, "tank-panther-down",
                          "./assets/images/tank-panther-down.png");
  _assetStore->AddTexture(_renderer, "chopper",
                          "./assets/images/chopper-spritesheet.png");

  _assetStore->AddTexture(_renderer, "jungle", "./assets/tilemaps/jungle.png");
  _assetStore->AddTexture(_renderer, "radar", "./assets/images/radar.png");

  _makeTilemap();

  // create entities
  Entity chopper = EntityManager::Get().CreateEntity();
  EntityManager::Get().AddComponent<TransformComponent>(
      chopper, glm::vec2(500.0, 100.0), glm::vec2(3.0, 3.0), 0);
  EntityManager::Get().AddComponent<RigidbodyComponent>(chopper,
                                                        glm::vec2(0.0, 0.0));
  EntityManager::Get().AddComponent<SpriteComponent>(chopper, "chopper", 32, 32,
                                                     2);
  EntityManager::Get().AddComponent<AnimationComponent>(chopper, 2, 5, true);
  EntityManager::Get().AddComponent<BoxColliderComponent>(chopper, 32, 32);
  EntityManager::Get().AddComponent<KeyboardControlledComponent>(
      chopper, 200., 200., 200., 200.);
  EntityManager::Get().AddComponent<CameraFollowComponent>(chopper);

  Entity radar = EntityManager::Get().CreateEntity();
  EntityManager::Get().AddComponent<TransformComponent>(
      radar, glm::vec2(200.0, 200.0), glm::vec2(2.0, 2.0), 0);
  EntityManager::Get().AddComponent<SpriteComponent>(radar, "radar", 64, 64, 2,
                                                     true);
  EntityManager::Get().AddComponent<AnimationComponent>(radar, 8, 3, true);
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

  // Reset all events handlers for the current frame
  _eventBus->Reset();

  // Perfom the subscription of the events for all systems
  _getEntityManager().GetSystem<DamageSystem>().SubscribeToEvents(_eventBus);
  _getEntityManager().GetSystem<KeyboardControlSystem>().SubscribeToEvents(
      _eventBus);

  // Update the entity manager to process the entities that are waiting to be
  // created/deleted
  _getEntityManager().Update();

  // Ask all the systems to update
  _getEntityManager().GetSystem<MovementSystem>().Update(deltaTime);
  _getEntityManager().GetSystem<AnimationSystem>().Update();
  _getEntityManager().GetSystem<CollisionSystem>().Update(_eventBus);
  _getEntityManager().GetSystem<CameraMovementSystem>().Update(camera);
}

void Game::Render() {

  SDL_SetRenderDrawColor(_renderer, 21, 21, 21, 255);
  SDL_RenderClear(_renderer);

  _getEntityManager().GetSystem<RenderSystem>().Update(_renderer, _assetStore,
                                                       camera);

  if (_debugMode) {
    _getEntityManager().GetSystem<RenderDebugSystem>().Update(_renderer, camera);
  }

  SDL_RenderPresent(_renderer);
}

void Game::Run() {

  Setup();

  // Game loop...
  while (_isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy() {
  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  SDL_Quit();
}

EntityManager &Game::_getEntityManager() { return EntityManager::Get(); }

void Game::_makeTilemap() const {
  std::vector<std::vector<int>> tileMatrix{};

  std::string line;
  std::ifstream mapFile;
  mapFile.open("./assets/tilemaps/jungle.map");

  int index = 0;

  if (mapFile.is_open()) {
    while (getline(mapFile, line)) {

      std::string tile{};

      tileMatrix.emplace_back(std::vector<int>{});

      for (size_t i = 0; i < line.size(); i++) {
        if (!std::isdigit(line[i])) {
          tileMatrix[index].emplace_back(std::stoi(tile));
          tile = "";
          continue;
        }

        tile += line[i];
      }

      if (!tile.empty()) {
        tileMatrix[index].emplace_back(std::stoi(tile));
      }

      index++;
    }

    mapFile.close();
  }

  // Actually draw the tilemap
  constexpr int tileSize{32};
  constexpr int tileScale{3};

  SDL_Surface *surface = IMG_Load("./assets/tilemaps/jungle.png");
  const int tileWidth = surface->w;
  SDL_FreeSurface(surface);

  // How many row the tilemap image has
  const int tilesPerRow = tileWidth / tileSize;

  for (size_t i{0}; i < tileMatrix.size(); i++) {
    for (size_t j{0}; j < tileMatrix[i].size(); j++) {

      // The index of the current tile
      const int tileIndex = tileMatrix[i][j];

      // Find what row in the tilemap I have to look
      const int row = floor(tileIndex / tilesPerRow);

      const int srcX = (tileIndex - (row * tilesPerRow)) * tileSize;
      const int srcY = row * tileSize;

      Entity tile = EntityManager::Get().CreateEntity();

      EntityManager::Get().AddComponent<TransformComponent>(
          tile,
          glm::vec2(((tileSize * tileScale) * j), ((tileSize * tileScale) * i)),
          glm::vec2(tileScale, tileScale));
      EntityManager::Get().AddComponent<SpriteComponent>(
          tile, "jungle", tileSize, tileSize, 0, false, srcX, srcY);
    }

    MapHeight = tileMatrix.size() * tileSize * tileScale;
    MapWidth = tileMatrix[0].size() * tileSize * tileScale;
  }
}

void Game::_initializeCameraView() {
  camera.x = 0;
  camera.y = 0;
  camera.w = WindowWidth;
  camera.h = WindowHeight;
}
