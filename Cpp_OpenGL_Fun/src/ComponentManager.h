#pragma once
#include "pch.h"
#include "Component.h"
#include "ComponentCreator.h"
#include "EntityManager.h"

namespace OpenGLFun {
	class ComponentManager {
	public:
		std::map<std::string, ComponentType> mComponentTypeMap; // find a component type by it's "registered" string
		std::map<ComponentType, IComponentCreator*> mComponentCreatorsMap; // find a creator by it's component type

		ComponentManager();
		~ComponentManager();

		void Clear();
		void AddComponent(IComponent*);
		void RemoveComponent(IComponent*);
		void RemoveComponent(EntityId const&, ComponentType const&);
		void RemoveComponents(EntityId const&);

		std::vector<IComponent*>& GetEntityComponents(EntityId const&);
		template<typename T>
		T* GetComponent(EntityId const&, ComponentType const&);
	private:
		std::map<EntityId, std::vector<IComponent*>> _mapEntityComponents;
	};
	extern ComponentManager* COMPONENT_MANAGER;

	template<typename T>
	T* ComponentManager::GetComponent(EntityId const& entityId, ComponentType const& componentType) {
		if (!ENTITY_MANAGER->HasComponent(entityId, componentType))
			return nullptr;

		for (IComponent* comp : _mapEntityComponents.at(entityId))
			if (comp->mCompType == componentType)
				return static_cast<T*>(comp);

		return nullptr;
	}
}