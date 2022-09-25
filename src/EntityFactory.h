#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"

namespace OpenGLFun {
	class EntityFactory {
	public:
		EntityFactory();
		~EntityFactory() {}

		EntityId CreateEntityFromFile(const char* filepath);
	};
	extern std::unique_ptr<EntityFactory> ENTITY_FACTORY;
}