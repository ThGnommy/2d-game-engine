#ifndef GAME_H
#define GAME_H

#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include <SDL2/SDL.h>
#include <memory>

static constexpr unsigned int FPS{60};
static constexpr unsigned int MILLISECS_PER_FRAME{1000 / FPS};

class EntityManager;

class Game {
public:
  Game();
  Game(Game &other) = delete;
  Game(const Game &&other) = delete;
  Game &operator=(Game &&other) = delete;
  Game &operator=(const Game &other) = delete;

  ~Game();

  void Initialize();
  void Run();
  void Setup();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  void LoadLevel(int level);

  int WindowWidth{};
  int WindowHeight{};

private:
  bool _isRunning{};
  bool _debugMode{};
  int millisecsPrevFrame{};
  SDL_Window *_window{};
  SDL_Renderer *_renderer{};

  std::unique_ptr<AssetStore> _assetStore{};
  std::unique_ptr<EventBus> _eventBus{};
  EntityManager &_getEntityManager();
};

#endif
