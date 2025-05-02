#include <iostream>
#include "Game.h"
#include "../ECS/ECS.h"
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"

Game::Game() {
	isRunning = false;
	registry = new Registry();
	window = NULL;
	renderer = NULL;
	windowWidth = 0;
	windowHeight = 0;
	Logger::set_level(Logger::level::trace);
	Logger::trace("Game constructor called!");
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
	Entity tank = registry->CreateEntity();
	Entity truck = registry->CreateEntity();
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

	//TODO:
	//MovementSystem.Update();
	// CollisionSystem.Update();
	// DamageSystem.Update();
}

void Game::Render() {
	//// Render update start
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render Entity's

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
