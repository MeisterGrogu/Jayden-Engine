#pragma once

#include <string>
#include "../Logger/Logger.h"

struct AnimationComponent {
	std::string animationName;
	unsigned int currentFrameIndex;
	double frameSpeedRate;
	bool isLoop;
	double frameTime;
	unsigned int layoutRows;

	AnimationComponent() = default;

	AnimationComponent(std::string animationName, double frameSpeedRate = 1, unsigned int layoutRows = 1, bool isLoop = true) {
		this->animationName = animationName;
		this->currentFrameIndex = 0;
		this->frameSpeedRate = frameSpeedRate;
		this->isLoop = isLoop;
		this->frameTime = 0;
		this->layoutRows = layoutRows;
		if (this->layoutRows == 0) {
			Logger::error("\"layoutCols\" cant be 0. Defaulting to 1");
			this->layoutRows = 1;
		}
	}
};
