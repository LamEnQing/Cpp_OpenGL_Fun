#include "pch.h"
#pragma once

namespace OpenGLFun {
	class EntityManager {
	public:
		EntityManager();
		~EntityManager();

		void Clear();

		std::set<EntityId> const& GetEntities() const;
		EntityId SpawnEntity();
		void MarkEntityDead(EntityId const& id);
		void RemoveDeadEntities();

		void SetSignatureBit(EntityId const& id, ComponentType compType, bool hasComponent);

		// uses entity's signature to test
		bool HasComponent(EntityId const& id, ComponentType type);
	private:
		std::queue<EntityId> _availableEntities;
		std::set<EntityId> _livingEntities;
		std::set<EntityId> _deadEntities;
		std::array<Signature, MAX_ENTITIES> _entitySignatures;
	};

	extern EntityManager* ENTITY_MANAGER;
}