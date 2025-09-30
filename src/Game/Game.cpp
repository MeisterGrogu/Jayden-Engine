#include "Game.h"
#include "../ECS/ECS.h"
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>
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
	assetHandler = std::make_unique<AssetHandler>();
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
		Logger::critical("Error initializing rendering.");
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
		0 | SDL_WINDOW_BORDERLESS // SDL_WINDOW_RESIZABLE // 
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

void Game::LoadLevel(int level) {
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderingSystem>();

	assetHandler->AddTexture(renderer, "tank-right", "./assets/images/tank-panther-right.png");
	assetHandler->AddTexture(renderer, "truck-down", "./assets/images/truck-ford-down.png");

	// TODO: I dont like this part loading the tilemap should be seperated and abstracted
	// TODO: into a Tilemap class in ECS.h so the user doesn't has to
	// TODO: pase it manualy and the AssetHandler should have e List for that
	assetHandler->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			// TODO: an Entity is really unefficient for this case instead make a Tile class in ECS.h
			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, srcRectX, srcRectY);
		}
	}

	mapFile.close();


	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(2.0, 2.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(80.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-right", 32, 32);

	Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 100.0));
	truck.AddComponent<SpriteComponent>("truck-down", 32, 32);
}

void Game::Setup() {
	LoadLevel(1);
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

	registry->GetSystem<RenderingSystem>().Update(renderer, assetHandler);

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
