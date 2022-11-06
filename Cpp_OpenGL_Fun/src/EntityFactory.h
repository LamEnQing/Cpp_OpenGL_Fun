#pragma once
#include "EntityManager.h"
#include "ComponentManager.h"

namespace OpenGLFun {
	class EntityFactory {
	public:
		EntityFactory();
		~EntityFactory() {}

		EntityId DeserializeEntity(std::string const& filename, rapidjson::Value const& jsonObj, bool allowParent=true);
		EntityId CreateEntityFromFile(std::string const& filepath);
	};
	extern std::unique_ptr<EntityFactory> ENTITY_FACTORY;
}