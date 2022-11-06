#include "LevelManager.h"
#include <filesystem>

#include "ComponentManager.h"
#include "Engine.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "FunImGuiSystem.h"
#include "GraphicSystem.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "Transform.h"
#include "FileOps.h"
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

	LevelManager::LevelManager() : mShouldReloadLevel{false}, mCurrentLevel{}, mLevels{} {}

	LevelManager::~LevelManager() {}

	void LevelManager::Load() {
		std::cout << "Level Manager Loading" << std::endl;
		std::cout << "----------------------------------------------------------------------" << std::endl;

		// scan levels, and add level folder names to vector
		for (const auto& entry : std::filesystem::directory_iterator(LEVEL_DIR)) {
			if (!entry.is_directory()) continue;

			mLevels.push_back(entry.path().filename().string());

			std::cout << "Found level " << mLevels.back() << '\n';
		}
		std::cout << std::endl;
	}

	void LevelManager::Unload() {
		mLevels.clear();
	}

	void LevelManager::ReloadLevel() {
		RESOURCE_MANAGER->UnloadTextures();
		RESOURCE_MANAGER->UnloadModels();

		COMPONENT_MANAGER->Clear();
		ENTITY_MANAGER->Clear();

		ENGINE->mConfig.ReloadConfig();
		LoadLevel(mCurrentLevel);
	}

	void LevelManager::LoadLevel(std::string const& levelId) {
		std::cout << "\nLoading a level, " << levelId << std::endl;
		std::cout << "----------------------------------------------------------------------" << std::endl;

		if (std::find(mLevels.begin(), mLevels.end(), levelId) == mLevels.end())
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
				Sprite* sprite = COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite);
				Texture* tex = RESOURCE_MANAGER->LoadTexture(sprite->mTextureFilepath);
				if (sprite->mUVDimensions[0] == 0)
					sprite->mUVDimensions[0] = tex->imgWidth;
				if (sprite->mUVDimensions[1] == 0)
					sprite->mUVDimensions[1] = tex->imgHeight;
			}
			if (COMPONENT_MANAGER->HasComponent(entityId, ComponentType::Model)) {
				ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(entityId, ComponentType::Model);
				if (!modelComp->mModelFilepath.empty()) {
					try {
						if (modelComp->mModelType == ModelType::TwoD)
							RESOURCE_MANAGER->Load2DModel(modelComp->mModelFilepath);
						else
							RESOURCE_MANAGER->Load3DModel(entityId, modelComp->mModelFilepath);
					}
					catch (std::exception& e) {
						throw SimpleException(std::string("Encountered an error reading a Model component:\n\t") + e.what());
					}
				}
			}
		}
		std::cout << std::endl;

		LoadLevelSetup(levelId);
	}

	int LevelManager::MaxLevel() const {
		return static_cast<int>(mLevels.size());
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

		if (document.HasMember("enable_editor")) {
			if (!document["enable_editor"].IsBool())
				throw JsonReadException(filepath, "enable_editor", "bool");
			FUN_IMGUI_SYSTEM->mShowEditor = document["enable_editor"].GetBool();
		}

		ENGINE->mPlayerId = ENTITY_FACTORY->DeserializeEntity(filepath, document["camera_entity"].GetObject(), false);
		if (COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Model)) {
			ModelComponent* modelComp = COMPONENT_MANAGER->GetComponent<ModelComponent>(ENGINE->mPlayerId, ComponentType::Model);
			if (!modelComp->mModelFilepath.empty()) {
				try {
					RESOURCE_MANAGER->Load3DModel(ENGINE->mPlayerId, modelComp->mModelFilepath);
				}
				catch (std::exception& e) {
					throw SimpleException(std::string("Encountered an error reading a Model component:\n\t") + e.what());
				}
			}
		}
		if (COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Sprite)) {
			RESOURCE_MANAGER->LoadTexture(COMPONENT_MANAGER->GetComponent<Sprite>(ENGINE->mPlayerId, ComponentType::Sprite)->mTextureFilepath);
		}

		if (document.HasMember("preload_assets")) {
			rapidjson::Value& preloadAssJson = document["preload_assets"];
			if (preloadAssJson.IsObject()) {
				RESOURCE_MANAGER->PreloadAsset(filepath, "preload_assets", preloadAssJson.GetObject());
			}
			else if (preloadAssJson.IsArray()) {
				for (rapidjson::SizeType i = 0; i < preloadAssJson.Size(); i++) {
					RESOURCE_MANAGER->PreloadAsset(filepath, std::string("preload_assets[") + std::to_string(i) + "]", preloadAssJson[i].GetObject());
				}
			}
			else {
				throw JsonReadException(filepath, "preload_assets", "JSON object or array");
			}
		}
	}
}