#pragma 
#include <memory>
#include <SDL.h>
#include "../ECS/ECS.h"
#include <glm/glm.hpp>
#include "../AssetManager/AssetHandler.h"

const int MAX_FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / MAX_FPS;

class Game {
	private:
		bool isRunning;
		Uint32 msPrevFrame = 0;
		SDL_Window* window;
		SDL_Renderer* renderer;

		std::unique_ptr<Registry> registry;
		std::unique_ptr<AssetHandler> assetHandler;

	public:
		Game(void);
		~Game(void);
		// TODO init takes title width heigth etc.
		void Initialize(void);
		void Run(void);
		void LoadLevel(int level);
		void Setup(void);
		void ProcessInput(void);
		void Update(void);
		void Render(void);
		void Destroy(void);

		int windowWidth;
		int windowHeight;
};