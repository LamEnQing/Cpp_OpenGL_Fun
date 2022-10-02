#include "LevelManager.h"
#include <filesystem>

#include "main.h"
#include "ComponentManager.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "GraphicSystem.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "Controllable.h"
#include "Transform.h"
#include "Serializer.h"
#include "Sprite.h"
#include "ModelComponent.h"

namespace OpenGLFun {
	const std::string LEVEL_DIR = "assets\\data\\levels";
	const std::string ENITY_DIR = "\\entity";

	LevelManager::LevelManager() : _currentLevel(0), _maxLevel(1), mPauseScreenObjs{} {
		/*for (const auto& entry : std::filesystem::directory_iterator(LEVEL_DIR)) {
			std::cout << "Discovered:" << entry.path().string() << '\n';
			try {
				ENTITY_FACTORY.get()->CreateEntityFromFile(entry.path().string().c_str());
			}
			catch (std::exception& e) {
				std::cout << "Failed to parse " << entry.path().string() << ", here's the error: " << e.what() << '\n';
			}
		}*/
	}

	LevelManager::~LevelManager() {}

	void LevelManager::Load() {
		std::cout << "Level Manager Loading\n";
		std::cout << "----------------------------------------------------------------------\n";
		for (const auto& entry : std::filesystem::directory_iterator("data")) {
			if (!Serializer::DoesFilenameEndWith(entry.path().string(), ".json")) {
				continue;
			}

			std::cout << "Discovered:" << entry.path().string() << '\n';
			try {
				ENTITY_FACTORY.get()->CreateEntityFromFile(entry.path().string().c_str());
			}
			catch (std::exception& e) {
				std::cout << "Failed to parse " << entry.path().string() << ", here's the error:\n\t" << e.what() << '\n';
			}
		}

		for (const auto& entry : std::filesystem::directory_iterator("data/pause_screen")) {
			if (!Serializer::DoesFilenameEndWith(entry.path().string(), ".json")) {
				continue;
			}

			std::cout << "Discovered:" << entry.path().string() << '\n';
			try {
				mPauseScreenObjs.push_back(ENTITY_FACTORY.get()->CreateEntityFromFile(entry.path().string().c_str()));
			}
			catch (std::exception& e) {
				std::cout << "Failed to parse " << entry.path().string() << ", here's the error:\n\t" << e.what() << '\n';
			}
		}

		engine->mPlayerId = ENTITY_MANAGER->SpawnEntity();

		Camera* camera = new Camera(engine->mPlayerId, { 0.f, 1.f, 0.f }, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
		COMPONENT_MANAGER->AddComponent(camera);

		Controllable* controllable = new Controllable(engine->mPlayerId);
		COMPONENT_MANAGER->AddComponent(controllable);

		Transform* transform = new Transform(engine->mPlayerId, { -3, 0, 0 }, {}, {});
		COMPONENT_MANAGER->AddComponent(transform);

		std::cout << "\nLoading Resources\n";
		std::cout << "--------------------------\n";
		// Load textures
		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite)) {
				RESOURCE_MANAGER->LoadTexture(COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite)->mTextureFilepath);
			}
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model)) {
				ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
				if (!modelComp->mModelFilepath.empty())
					RESOURCE_MANAGER->LoadModel(modelComp->mModelFilepath);
			}
		}
		RESOURCE_MANAGER->LoadTexture("no_texture.png");
		std::cout << "--------------------------\n";
	}

	void LevelManager::Unload() {
		RESOURCE_MANAGER->UnloadTextures();
		RESOURCE_MANAGER->UnloadModels();

		COMPONENT_MANAGER->Clear();
		ENTITY_MANAGER->Clear();
	}
}