#pragma once

#include <glm/detail/type_vec2.hpp>
#include <glm/fwd.hpp>

struct RigidBodyComponent {
	glm::vec2 velocity;

	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0)) {
		this->velocity = velocity;
	}
};