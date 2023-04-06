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
	void LoadLevelJson(std::string const& levelId);

	// these variables are private, only visible inside LevelManager.cpp
	const std::string LEVEL_DIR = "data\\level";
	
	// these variables are extern
	const std::string LEVEL_PAUSE_ID = "pause_screen";

	LevelManager::LevelManager() : mShouldReloadLevel{false}, mCurrentLevel{} {}

	LevelManager::~LevelManager() {}

	void LevelManager::ReloadLevel() {
		RESOURCE_MANAGER->UnloadTextures();
		RESOURCE_MANAGER->UnloadModels();

		COMPONENT_MANAGER->Clear();
		ENTITY_MANAGER->Clear();

		ENGINE->mConfig.ReloadConfig();
		LoadLevel(mCurrentLevel);
	}

	void LevelManager::LoadLevel(std::string const& levelId) {
		std::cout << std::endl << "Loading a level, " << levelId << std::endl;
		std::cout << "----------------------------------------------------------------------" << std::endl;

		std::cout << std::endl << "Preloading textures" << std::endl;
		std::cout << "----------------------------------------------------------------------" << std::endl;
		for (const std::string& textureFile : ENGINE->mConfig.mPreloadTextures) {
			RESOURCE_MANAGER->LoadTexture(textureFile);
		}

		LoadLevelJson(levelId);

		std::cout << std::endl << "Loading Resources" << std::endl;
		std::cout << "--------------------------" << std::endl;

		// Load textures
		for (EntityId const& entityId : ENTITY_MANAGER->GetEntities()) {
			if (ENTITY_MANAGER->HasComponent(entityId, ComponentType::Sprite)) {
				Sprite* sprite = COMPONENT_MANAGER->GetComponent<Sprite>(entityId, ComponentType::Sprite);
				Texture* tex = RESOURCE_MANAGER->LoadTexture(sprite->mTextureFilepath);
				if (sprite->mUVDimensions[0] == 0)
					sprite->mUVDimensions[0] = tex->imgWidth;
				if (sprite->mUVDimensions[1] == 0)
					sprite->mUVDimensions[1] = tex->imgHeight;
			}
			if (ENTITY_MANAGER->HasComponent(entityId, ComponentType::Model)) {
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
	}

	// Load setup for that level, should be called after level data has been loaded
	void LoadLevelJson(std::string const& levelId) {
		std::string filepath = LEVEL_DIR + "\\" + levelId;
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
			FUN_IMGUI_SYSTEM->ShowEditor(document["enable_editor"].GetBool());
		}

		ENGINE->mPlayerId = ENTITY_FACTORY->DeserializeEntity(filepath, document["camera_entity"].GetObject(), false);
		if (ENTITY_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Model)) {
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
		if (ENTITY_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Sprite)) {
			RESOURCE_MANAGER->LoadTexture(COMPONENT_MANAGER->GetComponent<Sprite>(ENGINE->mPlayerId, ComponentType::Sprite)->mTextureFilepath);
		}

		// ==== Load entities ====
		if (document.HasMember("entities")) {
			rapidjson::Value& entityArr = document["entities"];
			if (!entityArr.IsArray() || entityArr.Size() <= 0)
				throw JsonReadException(filepath, "entities", "JSON array with minimum size 1");

			for (rapidjson::SizeType i = 0; i < entityArr.Size(); i++) {
				rapidjson::Value& entityJson = entityArr[i];

				// check value type of the array element
				if (!entityJson.IsObject())
					throw JsonReadException(filepath, std::string("entities[") + std::to_string(i) + "]", "JSON object");

				// parse filename
				if (!entityJson.HasMember("filename") || !entityJson["filename"].IsString())
					throw JsonReadException(filepath, std::string("entities[") + std::to_string(i) + "]", "filename", "string");

				const char* entityFile = entityJson["filename"].GetString();
				bool hasErrors = false;
				try {
					EntityId entityId = ENTITY_FACTORY->CreateEntityFromFile(entityFile);

					// Check that entity has either Transform or Body component, in order to set the entity's position
					if (!ENTITY_MANAGER->HasComponent(entityId, ComponentType::Transform))
						throw JsonReadException(filepath, std::string("entities[") + std::to_string(i) + "]", entityFile, "must have either Transform or Body component");

					// can be nullptr, must check later on. BUT, only one of them can be nullptr, cannot be BOTH are nullptr
					Transform* transformComp = COMPONENT_MANAGER->GetComponent<Transform>(entityId, ComponentType::Transform);
					Vec3f posVector = /*bodyComp != nullptr ? bodyComp->mPosition :*/ transformComp->mPosition;

					// Parse position array
					if (entityJson.HasMember("position")) {
						if (!entityJson["position"].IsArray() || entityJson["position"].Size() < 2)
							throw JsonReadException(filepath, std::string("entities[") + std::to_string(i) + "]", "position", "JSON array of size 2");

						const rapidjson::Value& posArr = entityJson["position"];
						for (rapidjson::SizeType j = 0; j < 2; j++) {
							const rapidjson::Value& posValue = posArr[j];

							if (!posValue.IsNumber())
								throw JsonReadException(filepath, std::string("entities[") + std::to_string(i) + "]", std::string("position[") + std::to_string(j) + "]", "number");

							float value = posValue.GetFloat();

							if (j == 0)
								posVector.x = value;
							else
								posVector.y = value;
						}
					}
				} catch (std::exception& e) {
					// if there were errors with a malformed JSON entity, entity should not be created, by design with how the loading and all were coded
					// if there were errors with parsing a component, the entity and component should not be added, by design
					// if there were errors with loading a behaviour script, the behaviour script should not be added, but the entity will still be created
					if (!hasErrors) {
						std::cout << "====== START OF LEVEL MANAGER ERROR ======" << std::endl;
						hasErrors = true;
					}

					std::cout << "In level '" << filepath << "', failed to parse entity '" << entityFile << "', here's the error:\n\t" << e.what() << std::endl;
				}

				if (hasErrors)
					std::cout << "====== END OF LEVEL MANAGER ERROR ======" << std::endl;
			}
		}
	}
}