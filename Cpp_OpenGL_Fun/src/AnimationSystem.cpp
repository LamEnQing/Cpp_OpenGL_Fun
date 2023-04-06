#include "AnimationSystem.h"

#include "Camera.h"
#include "ComponentManager.h"
#include "Engine.h"
#include "EntityManager.h"
#include "FunImGuiSystem.h"
#include "ModelComponent.h"
#include "ResourceManager.h"
#include "Transform.h"

namespace OpenGLFun {
	AnimationSystem::AnimationSystem() {}
	AnimationSystem::~AnimationSystem() {}

	void AnimationSystem::Update(float const& deltaTime) {
		if((FUN_IMGUI_SYSTEM->ShowEditor() && ENGINE->mIsPaused) || ENGINE->mIsPaused) return;

		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			if (!ENTITY_MANAGER->HasComponent(entityId, ComponentType::Transform) || !ENTITY_MANAGER->HasComponent(entityId, ComponentType::Model))
				continue;

			ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
			if (modelComp->mModelType == ModelType::TwoD) continue;

			Model* model = RESOURCE_MANAGER->Get3DModel(modelComp->mModelFilepath);

			if (model == nullptr) continue;

			for (auto meshesIt = model->GetMeshMap().begin(); meshesIt != model->GetMeshMap().end(); meshesIt++) {
				std::string const& meshName = meshesIt->first;
				if (meshName == "leg_left" && entityId != ENGINE->mPlayerId) {
					Mesh* mesh = meshesIt->second.get();

					Vec3f rotation = mesh->GetRotation();
					rotation.z += 20 * deltaTime;

					if (rotation.z > 360)
						rotation.z = rotation.z - 360;

					mesh->SetRotation(rotation);
				}

				if (entityId == ENGINE->mPlayerId && meshName == "head" && ENTITY_MANAGER->HasComponent(entityId, ComponentType::Camera)) {
					//std::cout << "Rotating head" << std::endl;
					Camera* camera = COMPONENT_MANAGER->GetComponent<Camera>(entityId, ComponentType::Camera);

					// camRotation x value rotates about the y-axis
					Vec3f headRot(0.0f, glm::clamp(-camera->mCamRotation.x, -45.0f, 45.0f), camera->mCamRotation.y);
					meshesIt->second->SetRotation(headRot);
				}
			}
		}
	}
}