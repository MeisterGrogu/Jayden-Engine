#include "Game.h"
#include "../ECS/ECS.h"
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderingSystem.h"

Game::Game() {
	Logger::set_level(Logger::level::trace);
	Logger::trace("Game constructor called!");
	isRunning = false;
	registry = std::make_unique<Registry>();
	window = NULL;
	renderer = NULL;
	windowWidth = 0;
	windowHeight = 0;
}

Game::~Game(){
	Logger::trace("Game destructor called!");
}

void Game::Initialize(){
	//// Rendering init start
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		spdlog::critical("Error initializing rendering.");
		return;
	}
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;
	window = SDL_CreateWindow(
		"Jayden Engine",	
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		0 // SDL_WINDOW_RESIZABLE //| SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		spdlog::critical("Error creating window.");
		return;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!renderer) {
		spdlog::critical("Error creating renderer");
		return;
	}
	SDL_RenderSetLogicalSize(renderer, displayMode.w, displayMode.h);
	//// Rendering init stop

	isRunning = true;
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				// TODO remove
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				break;
		}
	}
}

void Game::Setup() {
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderingSystem>();

	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 50.0));
	tank.AddComponent<SpriteComponent>(10, 10);

	Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0));
	truck.AddComponent<SpriteComponent>(10, 50);
}

void Game::Update() {
	// limit FPS
	if (MAX_FPS != NULL) {
		int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - msPrevFrame);
		if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
			SDL_Delay(timeToWait);
		}
	}

	double deltaTime = (SDL_GetTicks() - msPrevFrame) / 1000.0;

	msPrevFrame = SDL_GetTicks();

	registry->GetSystem<MovementSystem>().Update(deltaTime);

	registry->Update();
}

void Game::Render() {
	//// Render update start
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderingSystem>().Update(renderer);

	SDL_RenderPresent(renderer);
	//// Render update stop
}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy(){
	//// Rendere quit start
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//// Rendere quit stop
	SDL_Quit();
}
