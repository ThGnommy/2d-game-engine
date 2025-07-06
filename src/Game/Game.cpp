#include "Game.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstddef>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include "../Components/TransformComponent.h"
#include "../Components/RigidbodyComponent.h" 

Game::Game() {
  isRunning = false;
  entityManager = std::make_unique<EntityManager>();
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
                       800, 600, SDL_WINDOW_ALWAYS_ON_TOP);

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

void Game::Setup() {
    Entity tank = entityManager->CreateEntity();
    entityManager->AddComponent<TransformComponent>(tank, glm::vec2(100.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    entityManager->AddComponent<RigidbodyComponent>(tank, glm::vec2(30.0, 0.0));
}

void Game::Update() {
  // If we are too fast, waste some time until we reach MILLISECS_PER_FRAME
  const unsigned int timeToWait =
      MILLISECS_PER_FRAME - (SDL_GetTicks64() - millisecsPrevFrame);

  if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
    SDL_Delay(timeToWait);
  }

  // double deltaTime = (SDL_GetTicks64() - millisecsPrevFrame) / 1000.0;
  millisecsPrevFrame = SDL_GetTicks64();

  // TODO: update game objects...

  // MovementSystem.Update();
  // ....
}

void Game::Render() {
  // 'Paint' the window
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
