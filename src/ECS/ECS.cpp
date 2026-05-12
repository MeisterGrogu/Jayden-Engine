#include "ECS.h"
#include <algorithm>
#include <string>
#include "../Logger/Logger.h"
#include <vector>

int IComponent::nextId = 0;

///////////////////
//// Entity
///////////////////

int Entity::GetId() const {
	return id;
}


///////////////////
//// System
///////////////////

void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
		return entity == other;
		}), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity& System::GetEntityFromSystem(int id)
{
	auto it = std::find_if(
		entities.begin(),
		entities.end(),
		[id](const Entity& e)
		{
			return e.GetId() == id;
		});

	if (it == entities.end())
	{
		throw std::runtime_error("Entity not found");
	}

	return *it;
}

Entity Registry::CreateEntity() {
	int entityId;
	entityId = numEntities++;
	Entity entity(entityId);
	entity.registry = this;
	entitesToBeAdded.insert(entity);

	if (entityId >= entityComponentSignatures.size()) {
		entityComponentSignatures.resize(static_cast<std::vector<Signature, std::allocator<Signature>>::size_type>(entityId) + 1);
	}

	Logger::trace("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	// Loop all the systems
	for (auto& system : systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::Update() {
	for (auto entity : entitesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitesToBeAdded.clear();


}
