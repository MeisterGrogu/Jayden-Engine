#pragma once


#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update(double deltaTime) {
		for (auto entity : GetSystemEnties()) {
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			const RigidBodyComponent rigidBody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidBody.velocity.x * deltaTime;
			transform.position.y += rigidBody.velocity.y * deltaTime;

			/*Logger::trace(
				"Entity id = " + std::to_string(entity.GetId())
				+ " position is now ("
				+ std::to_string(transform.position.x) + ", "
				+ std::to_string(transform.position.y) + ")"
			);*/
		}
	}
};