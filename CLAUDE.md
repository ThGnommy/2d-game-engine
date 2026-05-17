# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
make build   # Compile to ./gameengine (g++ C++17)
make run     # Execute the binary
make clean   # Remove build artifacts
```

Dependencies must be installed system-wide on Linux: `sdl2`, `sdl2_image`, `sdl2_ttf`, `sdl2_mixer`, `lua`. There is no test infrastructure.

## Architecture

The engine is a **data-oriented ECS (Entity Component System)** built on SDL2. The game loop runs at 60 FPS with delta time and calls systems in this order: MovementSystem → AnimationSystem → CollisionSystem → RenderSystem.

### ECS Core (`src/ECS/`)

- **EntityManager** (singleton, `EntityManager::Get()`) — owns all state: component pools, entity signatures, and system lists. Entity creation/deletion is deferred; queued changes are applied at the start of each frame via `EntityManager::Update()`.
- **Entity** — a lightweight `int id` handle. All component access (`AddComponent<T>`, `GetComponent<T>`, `HasComponent<T>`, `RemoveComponent<T>`) delegates to EntityManager.
- **Component<T>** — CRTP base that assigns each concrete component type a unique static integer ID at compile time. Components are plain data structs with no logic.
- **System** — base class; constructor calls `RequireComponent<T>()` to declare the component signature. EntityManager automatically adds/removes entities from each system when their component set changes. Systems expose `GetEntities()` and implement `Update()`.
- **Pool<T>** (`IPool.h`) — dense `std::vector<T>` indexed by entity ID. One pool per component type. Resizes dynamically.
- **Signature** (`Types.h`) — `std::bitset<32>` tracking which components an entity has. Max 32 component types.

### Adding a New System

1. Create a header in `src/Systems/`. Inherit from `System`.
2. Call `RequireComponent<YourComponent>()` in the constructor.
3. Implement `Update(...)`.
4. Register it in `Game::LoadLevel()` via `EntityManager::Get().AddSystem<YourSystem>()`.

### Adding a New Component

1. Create a struct in `src/Components/` that inherits from `Component<YourComponent>`.
2. Attach to an entity with `entity.AddComponent<YourComponent>(args...)`.

### Key subsystems

- **AssetStore** (`src/AssetStore/`) — `unordered_map` of `SDL_Texture*` keyed by string asset ID. Load textures with `AddTexture`, retrieve with `GetTexture`.
- **Logger** (`src/Logger/`) — static methods `Logger::Log`, `Logger::Warn`, `Logger::Err` with timestamps and ANSI color output.
- **GLM** (`libs/glm/`) — used for `glm::vec2` throughout. Other math types available but not yet used.
- **ImGui / Sol2 / Lua** — headers present in `libs/`, linked, but not wired into the engine yet.

### Assets

Sprite sheets, tilemaps, fonts, and sounds live in `assets/`. The tilemap loader in `Game.cpp` reads `assets/tilemaps/jungle.map` (integer tile indices) and renders rows/cols of tiles from `jungle.png`.
