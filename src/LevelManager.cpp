#include "LevelManager.h"
#include <filesystem>

#include "ComponentManager.h"
#include "main.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "Camera.h"
#include "Controllable.h"
#include "Transform.h"

namespace OpenGLFun {
	const std::string LEVEL_DIR = "assets\\data\\levels";
	const std::string ENITY_DIR = "\\entity";

	LevelManager::LevelManager() : _currentLevel(0), _maxLevel(1) {
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
		/*for (const auto& entry : std::filesystem::directory_iterator(LEVEL_DIR + std::to_string(_currentLevel) + "\\entity")) {
			std::cout << "Discovered:" << entry.path().string() << '\n';
			try {
				ENTITY_FACTORY.get()->CreateEntityFromFile(entry.path().string().c_str());
			}
			catch (std::exception& e) {
				std::cout << "Failed to parse " << entry.path().string() << ", here's the error: " << e.what() << '\n';
			}
		}*/
		for (const auto& entry : std::filesystem::directory_iterator("assets\\data")) {
			std::cout << "Discovered:" << entry.path().string() << '\n';
			try {
				ENTITY_FACTORY.get()->CreateEntityFromFile(entry.path().string().c_str());
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
	}

	void LevelManager::Unload() {
		COMPONENT_MANAGER->Clear();
		ENTITY_MANAGER->Clear();
	}
}