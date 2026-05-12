#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include <vector>
#include <SDL_rect.h>
#include <iterator>
#include <cmath>
#include <algorithm>

struct AABB {
	int entityId;
	unsigned int radius;
	SDL_Rect rect;
};

bool potentialCollide(AABB r1, AABB r2) {
	int dx = std::max(0, std::max(r1.rect.x, r2.rect.x) - std::min(r1.rect.x + r1.rect.w, r2.rect.x + r2.rect.w));
	int dy = std::max(0, std::max(r1.rect.y, r2.rect.y) - std::min(r1.rect.y + r1.rect.h, r2.rect.y + r2.rect.h));

	int distSq = (dx * dx) + (dy * dy);
	int radSum = r1.radius + r2.radius;

	return distSq <= (radSum * radSum);
}



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

			SDL_Rect rect = { static_cast<int>(transform.position.x + (boxCollider.offset.x * transform.scale.x)),
							static_cast<int>(transform.position.y + (boxCollider.offset.y * transform.scale.y)),
							static_cast<int>(boxCollider.width * transform.scale.x),
							static_cast<int>(boxCollider.height * transform.scale.y)
			};

			unsigned int radius = std::sqrt(boxCollider.width * boxCollider.width + boxCollider.height * boxCollider.height) / 2.0;

			aabbs.push_back({ e.GetId(), radius, rect});
		}

		for (auto i = aabbs.begin(); i != aabbs.end(); i++) {
			for (auto j = std::next(i); j != aabbs.end(); j++) {
				// Broad-phase
				if (!potentialCollide(*i, *j)) continue;

				// Narrow-phase
				if (SDL_HasIntersection(&i->rect, &j->rect)) {
					Logger::debug("Collision detected! Between {} and {}.", i->entityId, j->entityId);
				}
			}
		}

	}
};
