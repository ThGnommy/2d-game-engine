#include <cstddef>
#include <iostream>
#include <SDL2/SDL.h>
#include "Game.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

Game::Game()
{
  isRunning = false;
  std::cout << "Game constructor called!" << std::endl;
}

Game::~Game()
{
  std::cout << "Game destructor called!" << std::endl;
}

void Game::Initialize()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "Error initializing SDL." << std::endl;
    return;
  }

  // setup w and h
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = displayMode.w;
  windowHeight = displayMode.h;

  window = SDL_CreateWindow(
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      windowWidth,
      windowHeight,
      SDL_WINDOW_ALWAYS_ON_TOP);

  if (!window)
  {
    std::cerr << "Error creating SDL window." << std::endl;
    return;
  }

  // Create a 2D rendering context for a window.
  renderer = SDL_CreateRenderer(
      window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer)
  {
    std::cerr << "Error creating SDL renderer." << std::endl;
    return;
  }

  // Change the video mode to REAL fullscreen
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  isRunning = true;
}

void Game::ProcessInput()
{
  SDL_Event sdlEvent;

  // Poll the events
  while (SDL_PollEvent(&sdlEvent))
  {
    switch (sdlEvent.type)
    {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
      {
        isRunning = false;
      }
      break;
    }
  }
}

glm::vec2 playerPos{};
glm::vec2 playerVelocity{};

void Game::Setup()
{
  // TODO: Inizialize game objects...
  playerPos = glm::vec2(10., 20.);
  playerVelocity = glm::vec2(100., 0.);
}

void Game::Update()
{
  // If we are too fast, waste some time until we reach MILLISECS_PER_FRAME
  int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks64() - millisecsPrevFrame);


  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
  {
    SDL_Delay(timeToWait);
  }

  double deltaTime = (SDL_GetTicks64() - millisecsPrevFrame) / 1000.0;
  millisecsPrevFrame = SDL_GetTicks64();

  playerPos.x += playerVelocity.x * deltaTime;
  playerPos.y += playerVelocity.y * deltaTime;

  // TODO: update game objects...
}

void Game::Render()
{
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Load a png texture
  SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect dstRect = {
      static_cast<int>(playerPos.x),
      static_cast<int>(playerPos.y),
      32,
      32};
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);

  // 'Paint' the window
  SDL_RenderPresent(renderer);
}

void Game::Run()
{

  Setup();

  while (isRunning)
  {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::Destroy()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
