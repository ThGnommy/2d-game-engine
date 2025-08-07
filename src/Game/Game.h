#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <memory>

static constexpr unsigned int FPS{60};
static constexpr unsigned int MILLISECS_PER_FRAME{1000 / FPS};

class EntityManager;

class Game
{
public:
  Game();
  Game(Game& other) = delete;
  Game(const Game&& other) = delete;
  Game &operator=(Game&& other) = delete;
  Game &operator=(const Game& other) = delete;

  ~Game();

  void Initialize();
  void Run();
  void Setup();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  int windowWidth;
  int windowHeight;

private:
  bool isRunning;
  int millisecsPrevFrame{};
  SDL_Window *window;
  SDL_Renderer *renderer;

  EntityManager& _getEntityManager();
};

#endif
