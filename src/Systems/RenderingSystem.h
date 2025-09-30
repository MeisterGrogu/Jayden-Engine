#pragma once

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include <SDL.h>

class RenderingSystem : public System {
public:
	RenderingSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetHandler>& assetHandler) {
		for (auto entity : GetSystemEnties()) {
			const TransformComponent transform = entity.GetComponent<TransformComponent>();
			const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();

			SDL_Rect srcRect = sprite.srcRect;
			SDL_Rect dstRect = {
				transform.position.x,
				transform.position.y,
				sprite.width * transform.scale.x,
				sprite.height * transform.scale.y
			};
			
			SDL_RenderCopyEx(
				renderer,
				assetHandler->GetTexture(sprite.assetId),
				&srcRect,
				&dstRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE
			);
		}
	}
};
