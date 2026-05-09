#pragma once

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManager/AssetHandler.h"
#include <SDL_rect.h>
#include <memory>

class AnimationSystem : public System {
public:
	AnimationSystem() {
		RequireComponent<AnimationComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(double deltaTime, const std::unique_ptr<AssetHandler>& assetHandler) {
		for (auto& entity : GetSystemEntities())
		{
			SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
			AnimationComponent& animation = entity.GetComponent<AnimationComponent>();

            const auto& frames = assetHandler->GetAnimation(animation.animationName);
            if (frames.empty()) continue;

            int numFrames = frames.size();
            animation.frameTime += deltaTime * animation.frameSpeedRate;

            if (animation.isLoop) {
                animation.currentFrameIndex = static_cast<int>(animation.frameTime) % numFrames;
            }
            else {
                animation.currentFrameIndex = static_cast<int>(animation.frameTime);
                if (animation.currentFrameIndex >= numFrames) {
                    animation.currentFrameIndex = numFrames - 1;
                }
            }

            int actualFrame = frames[animation.currentFrameIndex];

            unsigned int gridX = actualFrame / animation.layoutRows;
            unsigned int gridY = actualFrame % animation.layoutRows;

            sprite.srcRect.x = gridX * sprite.width;
            sprite.srcRect.y = gridY * sprite.height;
		}
	}
};
