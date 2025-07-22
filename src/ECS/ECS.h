#pragma once

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <set>
#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Signature
///////////////////////////////////////////////////////////////////////////////////////////////////

// bitset (1s and 0s) to keep track of the components (like an ID)
typedef std::bitset<MAX_COMPONENTS> Signature;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Component
///////////////////////////////////////////////////////////////////////////////////////////////////

// parent for all the components
struct IComponent {
protected:
	static int nextId;
};

// for the assignment of an unique id to a component type
template <typename T>
class Component: public IComponent {
public:
	// returns the ID of Component<T>
	static int GetId() {
		static int id = nextId++;
		return id;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Entity
///////////////////////////////////////////////////////////////////////////////////////////////////

// class for all the entities (only an numerical ID)
class Entity {
private:
	int id;

public:
	Entity(int id) : id(id) {};
	Entity(const Entity& entity) = default;
	// Get the ID of an Entity
	int GetId() const;

	Entity& operator =(const Entity& other) = default;
	bool operator ==(const Entity& other) const { return id == other.id; }
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }
	bool operator >(const Entity& other) const { return id > other.id; }

	// adds a component of type TComponent with the arguments TArgs
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	// removes an component of type TComponent
	template <typename TComponent> void RemoveComponent();
	// checks if an entity has a component of type TComponent
	template <typename TComponent> bool HasComponent() const;
	// returns a reference to a specific component from an Entity
	template <typename TComponent> TComponent& GetComponent() const;

	class Registry* registry;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////////////////////////////////

// processes entities that contain a specific signature (logic)
class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEnties() const; // getting the Entities in the System
	const Signature& GetComponentSignature() const; // getting the signature of the Components assigned to this System

	// component which entities need if the system should run at them
	template <typename TComponent> void RequireComponent();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Pool
///////////////////////////////////////////////////////////////////////////////////////////////////

// Inheritance Pool class so you don't have to specify the type of the Pool in the registry
class IPool {
public:
	virtual ~IPool() {}
};


// a vector of objects of type T
template <typename T> class Pool: public IPool {
private:
	std::vector<T> data;
public:
	Pool(int size = 100) {
		data.resize(size);
	}
	virtual ~Pool() = default;

	bool IsEmpty() const {
		return data.empty();
	}

	size_t GetSize() const {
		return data.size();
	}

	void Resize(int n) {
		data.resize(n);
	} 

	void Clear() {
		data.clear();
	}

	void Add(T object) {
		data.push_back(object);
	}

	void Set(int index, T object) {
		data[index] = object;
	}

	T& Get(int index) {
		return static_cast<T&>(data[index]);
	}

	T& operator [](unsigned int index) {
		return data[index];
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Registry
///////////////////////////////////////////////////////////////////////////////////////////////////

// manages the whole Entity-Component-System System (main Manager)
class Registry {
private:
	int numEntities = 0;
	std::set<Entity> entitesToBeAdded; // Entities waiting for creation in next Update()
	std::set<Entity> entitesToBeRemoved; // Entities waiting for destruction in next Update()

	// Vector of component pools, each pool contains all the data for a certain component type
	// [Vector index = component type id]
	// [Pool index = entity id]
	std::vector<std::shared_ptr<IPool>> componentPools;

	// list which holds information about which component is used for which entity 
	// [vector index = entity id]
	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
	Registry() {
		Logger::trace("Registry constructor called!");
	} // = default;

	~Registry() {
		Logger::trace("Registry destructor called!");
	} // = default;


	void Update();
	Entity CreateEntity();
	//void AddEntityToSystem(Entity entity);
	//void KillEntity(Entity entity);

	// adds a component of type TComponent with the arguments TArgs
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	// removes an component of type TComponent
	template <typename TComponent> void RemoveComponent(Entity entity);
	// checks if an entity has a component of type TComponent
	template <typename TComponent> bool HasComponent(Entity entity) const;
	// returns a reference to a specific component from an Entity
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;	

	// adds a system of type TSystem with the arguments TArgs
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	// removes a system of type TSystem
	template <typename TSystem> void RemoveSystem();
	// checks if the game has a system of type TSystem
	template <typename TSystem> bool HasSystem() const;
	// returns the system of type TSystem from the registry
	template <typename TSystem> TSystem& GetSystem() const;

	// Checks the component Signature of an entity and add the entity to the
	// that are interested in it
	void AddEntityToSystems(Entity entity);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of template functions
///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TComponent>
void System::RequireComponent() {
	const int componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& Registry::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}


template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const int componentId = Component<TComponent>::GetId();
	const int entityId = entity.GetId();

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	entityComponentSignatures[entityId].set(componentId);

	Logger::debug("Component id = " + std::to_string(componentId) + " was added to entity id " + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const int componentId = Component<TComponent>::GetId();
	const int entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId, false);

	Logger::debug("Component id = " + std::to_string(componentId) + " was removed from entity id " + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const int componentId = Component<TComponent>::GetId();
	const int entityId = entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const int componentId = Component<TComponent>::GetId();
	const int entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool->Get(entityId);	
}

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
	return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent() const {
	return registry->GetComponent<TComponent>(*this);
}

