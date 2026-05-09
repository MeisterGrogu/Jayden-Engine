#pragma once
#include <map>
#include <unordered_set>
#include <SDL.h>
#include "../ECS/ECS.h"
#include <string>
#include <vector>

class AssetHandler {
private:
	std::map<std::string, SDL_Texture*> textures;
	std::unordered_set<std::string> illegalTextures;

	std::map<std::string, Animation> animations;

public:
	AssetHandler();
	~AssetHandler();

	void ClearAssets();
	
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string & assetId);

	void AddAnimation(const std::string& animationId, const Animation& frameIndices);
	void AddAnimation(const std::string& animationId, const unsigned int frameFrom, const unsigned int frameTo);
	void AddAnimation(const std::string& animationId, const unsigned int frames);

	const Animation& GetAnimation(const std::string& animationId) const;
};
