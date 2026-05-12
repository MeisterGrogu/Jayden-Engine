#pragma once
#include <glm/fwd.hpp>

struct BoxColliderComponent {
	int width;
	int height;
	glm::vec2 offset;

	BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = { 0,0 }) {
		this->width = width;
		this->height = height;
		this->offset = offset;
	}
};
