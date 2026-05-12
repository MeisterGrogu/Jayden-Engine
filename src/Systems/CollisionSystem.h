#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <vector>
#include <SDL_rect.h>
#include <iterator>

struct AABB {
	int entityId;
	SDL_Rect rect;
};

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update() {
		auto entities = GetSystemEntities();
		std::vector<AABB> aabbs;

		for (auto e : entities) {
			auto& boxCollider = e.GetComponent<BoxColliderComponent>();
			auto& transform = e.GetComponent<TransformComponent>();

			SDL_Rect rect = { static_cast<int>(transform.position.x + (boxCollider.offset.x * transform.scale.x)) ,
							static_cast<int>(transform.position.y + (boxCollider.offset.y * transform.scale.y)),
							static_cast<int>(boxCollider.width * transform.scale.x),
							static_cast<int>(boxCollider.height * transform.scale.y)
			};

			aabbs.push_back({ e.GetId(), rect});
		}

		for (auto i = aabbs.begin(); i != aabbs.end(); i++) {
			auto& aabbA = *i;
			for (auto j = std::next(i); j != aabbs.end(); j++) {
				auto& aabbB = *j;

				if (SDL_HasIntersection(&aabbA.rect, &aabbB.rect)) {
					Logger::debug("Collision detected! Between {} and {}.", aabbA.entityId, aabbB.entityId);
					// Entity& eA = GetEntityFromSystem(aabbA.entityId);
					// Entity& eB = GetEntityFromSystem(aabbB.entityId);
				}
			}
		}
	}
};
