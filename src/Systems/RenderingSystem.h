#pragma once

#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class RenderingSystem : public System {
public:
	RenderingSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer) {
		for (auto entity : GetSystemEnties()) {
			const TransformComponent transform = entity.GetComponent<TransformComponent>();
			const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
			
			SDL_Rect objRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &objRect);
		}
	}
};