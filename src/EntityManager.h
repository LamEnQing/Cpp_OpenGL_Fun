#include "pch.h"
#pragma once

namespace OpenGLFun {
	class EntityManager {
	public:
		EntityManager();
		~EntityManager();

		void Clear();

		std::vector<EntityId> const& GetEntities() const;
		EntityId SpawnEntity();
		void MarkEntityDead(EntityId const& id);
		void RemoveDeadEntities();
	private:
		std::queue<EntityId> _availableEntities;
		std::vector<EntityId> _livingEntities;
		std::vector<EntityId> _deadEntities;
	};

	extern EntityManager* ENTITY_MANAGER;
}