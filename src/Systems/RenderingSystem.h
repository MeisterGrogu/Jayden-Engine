#pragma once

#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include <algorithm>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>

struct RenderableEntity {
	TransformComponent transformComponent;
	SpriteComponent spriteComponent;
};

class RenderingSystem : public System {
public:
	RenderingSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetHandler>& assetHandler) {

		std::vector<RenderableEntity> renderableEntities;

		for (auto& entity : GetSystemEntities()) {
			RenderableEntity renderableEntity;
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
			renderableEntities.emplace_back(renderableEntity);
		}

		std::sort(renderableEntities.begin(), renderableEntities.end(),
			[](const RenderableEntity& a, const RenderableEntity& b) {
				return a.spriteComponent.zIndex <
					b.spriteComponent.zIndex;
			});

		for (auto& entity : renderableEntities) {
			const auto& transform = entity.transformComponent;
			const auto& sprite = entity.spriteComponent;

			SDL_Rect srcRect = sprite.srcRect;

			SDL_Rect dstRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
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
