#pragma once

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;
	int zIndex;
	SDL_Rect srcRect;

	SpriteComponent(const std::string& assetId = "", int width = 1, int height = 1, int zIndex = 0, int srcRectX = 0, int srcRectY = 0) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->zIndex = zIndex;
		this->srcRect = { srcRectX, srcRectY, width, height };
	}
};