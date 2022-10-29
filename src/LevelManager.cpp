#include "LevelManager.h"
#include <filesystem>

#include "ComponentManager.h"
#include "Engine.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "GraphicSystem.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "Transform.h"
#include "Serializer.h"
#include "Sprite.h"
#include "ModelComponent.h"

namespace OpenGLFun {
	void LoadLevelSetup(std::string const& levelId);

	// these variables are private, only visible inside LevelManager.cpp
	const std::string LEVEL_DIR = "data\\levels";
	const std::string ENTITY_SUBDIR = "\\entity";
	const std::string LEVEL_SETUP_JSON = "setup.json";
	
	// these variables are extern
	const std::string LEVEL_PAUSE_ID = "pause_screen";

	LevelManager::LevelManager() : mCurrentLevel{}, _levels{} {}

	LevelManager::~LevelManager() {}

	void LevelManager::Load() {
		std::cout << "Level Manager Loading" << std::endl;
		std::cout << "----------------------------------------------------------------------" << std::endl;

		// scan levels, and add level folder names to vector
		for (const auto& entry : std::filesystem::directory_iterator(LEVEL_DIR)) {
			if (!entry.is_directory()) continue;

			_levels.push_back(entry.path().filename().string());

			std::cout << "Found level " << _levels.back() << '\n';
		}
		std::cout << std::endl;
	}

	void LevelManager::Unload() {
		_levels.clear();
	}

	void LevelManager::ReloadLevel() {
		RESOURCE_MANAGER->UnloadTextures();
		RESOURCE_MANAGER->UnloadModels();

		COMPONENT_MANAGER->Clear();
		ENTITY_MANAGER->Clear();

		LoadLevel(mCurrentLevel);
	}

	void LevelManager::LoadLevel(std::string const& levelId) {
		std::cout << "\nLoading a level, " << levelId << std::endl;
		std::cout << "----------------------------------------------------------------------" << std::endl;

		if (std::find(_levels.begin(), _levels.end(), levelId) == _levels.end())
			throw SimpleException(std::string("Could not find level '") + levelId + "'");

		for (const auto& entry : std::filesystem::directory_iterator(LEVEL_DIR + "\\" + levelId + ENTITY_SUBDIR)) {
			if (!Serializer::DoesFilenameEndWith(entry.path().string(), ".json")) continue;

			std::cout << "Discovered:" << entry.path().string() << '\n';
			try {
				ENTITY_FACTORY.get()->CreateEntityFromFile(entry.path().string());
			}
			catch (std::exception& e) {
				throw SimpleException(std::string("Failed to parse ") + entry.path().string() + ", here's the error:\n\t" + e.what() + '\n');
			}
		}

		std::cout << std::endl << "Loading Resources" << std::endl;
		std::cout << "--------------------------" << std::endl;

		// Load textures
		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Sprite)) {
				RESOURCE_MANAGER->LoadTexture(COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite)->mTextureFilepath);
			}
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model)) {
				ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
				if (!modelComp->mModelFilepath.empty()) {
					try {
						RESOURCE_MANAGER->LoadModel(entityId, modelComp->mModelFilepath);
					}
					catch (std::exception& e) {
						throw SimpleException(std::string("Encountered an error reading a Model component:\n\t") + e.what());
					}
				}
			}
		}
		RESOURCE_MANAGER->LoadTexture("no_texture.png");
		std::cout << std::endl;

		LoadLevelSetup(levelId);
	}

	int LevelManager::MaxLevel() const {
		return static_cast<int>(_levels.size());
	}

	// Load setup for that level, should be called after level data has been loaded
	void LoadLevelSetup(std::string const& levelId) {
		std::string filepath = LEVEL_DIR + "\\" + levelId + "\\" + LEVEL_SETUP_JSON;
		std::string json_from_file = Serializer::GetFileContents(filepath.c_str());

		rapidjson::Document document;
		if (document.Parse(json_from_file.c_str()).HasParseError()) {
			// I put filepath into std::string, so that I can append new strings or char pointers via add operator
			throw SimpleException(filepath + " has an incorrect JSON format");
		}

		if (!document.IsObject())
			throw SimpleException(filepath + " must start with a JSON object");

		if (!document.HasMember("camera_entity") || !document["camera_entity"].IsObject()) {
			throw JsonReadException(filepath, "camera_entity", "JSON object");
		}

		if (document.HasMember("should_mouse_lock")) {
			if (!document["should_mouse_lock"].IsBool())
				throw JsonReadException(filepath, "should_mouse_lock", "bool");
			ENGINE->mShouldMouseBeLocked = document["should_mouse_lock"].GetBool();
		}

		ENGINE->mPlayerId = ENTITY_FACTORY->DeserializeEntity(filepath, document["camera_entity"].GetObject(), false);
		if (COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Model)) {
			ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(ENGINE->mPlayerId, ComponentType::Model);
			if (!modelComp->mModelFilepath.empty()) {
				try {
					RESOURCE_MANAGER->LoadModel(ENGINE->mPlayerId, modelComp->mModelFilepath);
				}
				catch (std::exception& e) {
					throw SimpleException(std::string("Encountered an error reading a Model component:\n\t") + e.what());
				}
			}
		}
		if (COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Sprite)) {
			RESOURCE_MANAGER->LoadTexture(COMPONENT_MANAGER->GetComponent<Sprite>(ENGINE->mPlayerId, ComponentType::Sprite)->mTextureFilepath);
		}
	}
}