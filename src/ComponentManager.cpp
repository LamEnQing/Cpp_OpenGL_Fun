#include "ComponentManager.h"

#include "Button.h"
#include "Color.h"
#include "ModelComponent.h"
#include "Sprite.h"
#include "Transform.h"
#include "Camera.h"

namespace OpenGLFun {
	ComponentManager::ComponentManager() {
		mComponentCreatorsMap.insert({ "Button", new ComponentCreator<Button>() });
		mComponentCreatorsMap.insert({ "Camera", new ComponentCreator<Camera>() });
		mComponentCreatorsMap.insert({ "Color", new ComponentCreator<Color>() });
		mComponentCreatorsMap.insert({ "Model", new ComponentCreator<ModelComponent>() });
		mComponentCreatorsMap.insert({ "Sprite", new ComponentCreator<Sprite>() });
		mComponentCreatorsMap.insert({ "Transform", new ComponentCreator<Transform>() });
	}

	ComponentManager::~ComponentManager() {
		Clear();

		for (auto creatorIt = mComponentCreatorsMap.rbegin(); creatorIt != mComponentCreatorsMap.rend(); creatorIt++)
			delete creatorIt->second;
		mComponentCreatorsMap.clear();
	}

	void ComponentManager::Clear() {
		for (auto entityIt = _mapEntityComponents.rbegin(); entityIt != _mapEntityComponents.rend(); entityIt++) {
			for (auto compIt = entityIt->second.rbegin(); compIt != entityIt->second.rend(); compIt++)
				delete *compIt;
		}
		_mapEntityComponents.clear();
	}

	void ComponentManager::AddComponent(IComponent* component) {
		// could not find entity id, so we add a new entry
		if (_mapEntityComponents.find(component->mOwner) == _mapEntityComponents.end()) {
			_mapEntityComponents.insert({ component->mOwner, { component } });
		}
		else {
			if (HasComponent(component->mOwner, component->mCompType))
				throw SimpleException(std::string("Component type ") + std::to_string(static_cast<int>(component->mCompType)) + " added to Entity " + std::to_string(component->mOwner) +  " already");
			_mapEntityComponents.at(component->mOwner).push_back(component);
		}
	}

	void ComponentManager::RemoveComponent(IComponent* comp) {
		if (_mapEntityComponents.find(comp->mOwner) == _mapEntityComponents.end())
			return;

		std::vector<IComponent*>& entityComponents = _mapEntityComponents.at(comp->mOwner);
		entityComponents.erase(std::remove(entityComponents.begin(), entityComponents.end(), comp), entityComponents.end());

		delete comp;
	}

	void ComponentManager::RemoveComponent(EntityId const& entityId, ComponentType const& componentType) {
		RemoveComponent(GetComponent<IComponent>(entityId, componentType));
	}

	void ComponentManager::RemoveComponents(EntityId const& entityId) {
		if (_mapEntityComponents.find(entityId) == _mapEntityComponents.end())
			return;

		for (IComponent* comp : _mapEntityComponents.at(entityId))
			RemoveComponent(comp);
	}

	bool ComponentManager::HasComponent(EntityId const& entityId, ComponentType const& compType) {
		if (_mapEntityComponents.find(entityId) == _mapEntityComponents.end())
			return false;

		for (IComponent* comp : _mapEntityComponents.at(entityId)) {
			if (comp->mCompType == compType)
				return true;
		}

		return false;
	}

	std::vector<IComponent*>& ComponentManager::GetEntityComponents(EntityId const& entityId) {
		static std::vector<IComponent*> emptyCompVector{};
		if (_mapEntityComponents.find(entityId) == _mapEntityComponents.end())
			return emptyCompVector;

		return _mapEntityComponents.at(entityId);
	}
}