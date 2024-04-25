#pragma once

#include <SDL2/SDL.h>

static constexpr unsigned int FPS{60};
static constexpr unsigned int MILLISECS_PER_FRAME{1000 / FPS};

class Game
{
private:
  bool isRunning;
  int millisecsPrevFrame{};
  SDL_Window *window;
  SDL_Renderer *renderer;

public:
  Game();
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
};
