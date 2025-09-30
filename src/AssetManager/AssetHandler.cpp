#include "AssetHandler.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>

AssetHandler::AssetHandler() {
	Logger::trace("AssetHandler constructor called!");
}

AssetHandler::~AssetHandler() {
	ClearAssets();
	Logger::trace("AssetHandler destructor called!");
}

void AssetHandler::ClearAssets() {
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetHandler::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	textures.emplace(assetId, texture);

	Logger::debug("New Texture with id: \"" + assetId + "\" was added to the Asset Handler!");
}

SDL_Texture* AssetHandler::GetTexture(const std::string& assetId) {
	return textures[assetId];
}