#include "AnimationSystem.h"

#include "ComponentManager.h"
#include "Engine.h"
#include "EntityManager.h"
#include "ModelComponent.h"
#include "ResourceManager.h"
#include "Transform.h"

namespace OpenGLFun {
	AnimationSystem::AnimationSystem() {}
	AnimationSystem::~AnimationSystem() {}

	void AnimationSystem::Update(float const& deltaTime) {
		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			std::cout << "entityId:" << entityId << ',' << (entityId == ENGINE->mPlayerId) << std::endl;
			if (entityId == ENGINE->mPlayerId || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Transform) || !COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model))
				continue;

			ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
			if (modelComp->mModelType == ModelType::TwoD) continue;

			Transform* transform = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);
			Model* model = RESOURCE_MANAGER->GetModel(entityId);

			for (auto meshesIt = model->GetMeshMap().begin(); meshesIt != model->GetMeshMap().end(); meshesIt++) {
				std::string const& meshName = meshesIt->first;
				if (meshName.find("leg") != std::string::npos && meshName.find("left") != std::string::npos) {
					Mesh* mesh = meshesIt->second.get();

					Vec3f rotation = mesh->GetRotation();
					rotation.z += 20 * deltaTime;

					if (rotation.z > 360)
						rotation.z = rotation.z - 360;

					mesh->SetRotation(rotation);
				}
			}
		}
	}
}