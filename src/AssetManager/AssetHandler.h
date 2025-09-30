#pragma once
#include <map>
#include <vector>
#include <SDL.h>
#include <string>

class AssetHandler {
private:
	std::map<std::string, SDL_Texture*> textures;

public:
	AssetHandler();
	~AssetHandler();

	void ClearAssets();
	
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string & assetId);
};
