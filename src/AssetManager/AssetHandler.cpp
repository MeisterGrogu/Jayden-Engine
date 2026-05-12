#include "AssetHandler.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <vector>
#include <numeric>
#include <string>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

AssetHandler::AssetHandler() {
	Logger::trace("AssetHandler constructor called!");
}

AssetHandler::~AssetHandler() {
	ClearAssets();
	Logger::trace("AssetHandler destructor called!");
}

void AssetHandler::ClearAssets() {
	for (auto& texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetHandler::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	if (surface) {
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	else {
		Logger::error("Path \"{}\" for \"{}\" doesnt exist!", filePath, assetId);
		illegalTextures.insert(assetId); // Mark as illegal here
		return;
	}

	textures.emplace(assetId, texture);
	Logger::debug("New Texture with id: \"" + assetId + "\" was loaded into the Asset Handler!");
}

SDL_Texture* AssetHandler::GetTexture(const std::string& assetId) {
	auto tex = textures[assetId];
	if (illegalTextures.count(assetId)) {
		Logger::error("Texture \"" + assetId + "\" could not be loaded!");
		illegalTextures.insert(assetId);
	}
	return tex;
}

void AssetHandler::AddAnimation(const std::string& animationId, const Animation& frameIndices) {
	animations[animationId] = frameIndices;
	if (frameIndices.empty()) {
		Logger::warn("The animation \"{}\" is empty", animationId);
	}
	Logger::debug("New Animation \"" + animationId + "\" added with " + std::to_string(frameIndices.size()) + " frames.");
}

void AssetHandler::AddAnimation(const std::string& animationId, const unsigned int framesFrom, const unsigned int framesTo) {
	std::vector<int> frames(framesTo - framesFrom);
	std::iota(frames.begin(), frames.end(), framesFrom);
	AddAnimation(animationId, frames);
}

void AssetHandler::AddAnimation(const std::string& animationId, const unsigned int frames) {
	AddAnimation(animationId, 0, frames);
}


const std::vector<int>& AssetHandler::GetAnimation(const std::string& animId) const {
	auto it = animations.find(animId);
	if (it == animations.end()) {
		Logger::error("Animation \"" + animId + "\" not found!");
		static const std::vector<int> empty;
		return empty;
	}
	return it->second;
}
