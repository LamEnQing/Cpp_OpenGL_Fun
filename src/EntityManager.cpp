#include "EntityManager.h"
#include "ComponentManager.h"

namespace OpenGLFun {
	EntityManager::EntityManager() {
		for (EntityId i = 0; i < MAX_ENTITIES; i++)
			_availableEntities.push(i);
	}

	EntityManager::~EntityManager() {}

	std::vector<EntityId> const& EntityManager::GetEntities() const {
		return _livingEntities;
	}

	void EntityManager::Clear() {
		std::vector<EntityId> copyLiving = _livingEntities;
		for (EntityId const& entityId : copyLiving)
			MarkEntityDead(entityId);
		copyLiving.clear();

		RemoveDeadEntities();
	}

	EntityId EntityManager::SpawnEntity() {
		if (_availableEntities.size() <= 0)
			throw SimpleException("Ran out of EntityIds!");

		EntityId id = _availableEntities.front(); // get id from the queue
		_availableEntities.pop(); // removes the id from the queue

		_livingEntities.push_back(id);

		return id;
	}

	void EntityManager::MarkEntityDead(EntityId const& id) {
		// find() reached the end of the vector, meaning it could not find the id
		if (std::find(_livingEntities.begin(), _livingEntities.end(), id) == _livingEntities.end())
			return;

		std::cout << "Marked " << std::to_string(id) << " for removal\n";

		// Remove entity from living entities
		_livingEntities.erase(std::remove(_livingEntities.begin(), _livingEntities.end(), id), _livingEntities.end());

		// Entity is queued to be removed
		_deadEntities.push_back(id);
	}

	void EntityManager::RemoveDeadEntities() {
		for (EntityId& id : _deadEntities) {
			COMPONENT_MANAGER->RemoveComponents(id);

			// Return id to availableEntities
			_availableEntities.push(id);
		}

		// After removing components, remove id from deadEntities
		_deadEntities.clear();
	}
}