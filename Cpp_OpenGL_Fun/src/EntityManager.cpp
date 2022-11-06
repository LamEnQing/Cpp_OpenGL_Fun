#include "EntityManager.h"
#include "ComponentManager.h"

namespace OpenGLFun {
	EntityManager::EntityManager() {
		for (EntityId i = 0; i < MAX_ENTITIES; i++)
			_availableEntities.push(i);
	}

	EntityManager::~EntityManager() {}

	std::set<EntityId> const& EntityManager::GetEntities() const {
		return _livingEntities;
	}

	void EntityManager::Clear() {
		for (EntityId const& entityId : _livingEntities)
			MarkEntityDead(entityId);

		RemoveDeadEntities();
		_livingEntities.clear();
	}

	EntityId EntityManager::SpawnEntity() {
		if (_availableEntities.size() <= 0)
			throw SimpleException("Ran out of EntityIds!");

		EntityId id = _availableEntities.front(); // get id from the queue
		_availableEntities.pop(); // removes the id from the queue

		_livingEntities.insert(id);

		return id;
	}

	void EntityManager::MarkEntityDead(EntityId const& id) {
		// find() reached the end of the vector, meaning it could not find the id
		if (std::find(_livingEntities.begin(), _livingEntities.end(), id) == _livingEntities.end())
			return;

		std::cout << "Marked " << std::to_string(id) << " for removal\n";

		// Entity is queued to be removed
		_deadEntities.insert(id);
	}

	void EntityManager::RemoveDeadEntities() {
		for (EntityId const& id : _deadEntities) {
			COMPONENT_MANAGER->RemoveComponents(id);

			// Remove entity from living entities
			_livingEntities.erase(id);

			// Return id to availableEntities
			_availableEntities.push(id);
		}

		// After removing components, remove id from deadEntities
		_deadEntities.clear();
	}

	void EntityManager::SetSignatureBit(EntityId const& id, ComponentType compType, bool hasComponent) {
		_entitySignatures[id].set(static_cast<int>(compType), hasComponent);
	}

	bool EntityManager::HasComponent(EntityId const& id, ComponentType type) {
		return _entitySignatures[id].test(static_cast<int>(type));
	}
}