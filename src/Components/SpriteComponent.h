#pragma once

struct SpriteComponent {
	int width;
	int height;

	SpriteComponent(int width = 1, int height = 1) {
		this->width = width;
		this->height = height;
	}
};