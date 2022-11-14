#pragma once
#include "pch.h"

#include "EntityManager.h"
#include "Model.h"
#include "Texture.h"

namespace OpenGLFun {
	class ResourceManager {
	public:
		std::map<std::string, std::shared_ptr<Texture>> mTexturesDataMap;

		ResourceManager();
		~ResourceManager();

		void PreloadAsset(std::string const& parentFile, std::string const& dataMember, rapidjson::Value& jsonObj);

		Texture* LoadTexture(std::string textureFilepath);
		void UnloadTextures();
		Texture* GetTexture(std::string textureFilepath);

		Mesh* LoadMesh(std::string modelFilepath);

		Model* Load2DModel(std::string modelFilepath);
		Model* Get2DModel(std::string const& modelFilepath);
		std::shared_ptr<Model> Load3DModel(EntityId const& entityId, std::string modelFilepath);
		std::shared_ptr<Model>& Get3DModel(EntityId const& entityId);
		void UnloadModels();
	private:
		std::map<EntityId, std::shared_ptr<Model>> _3DModelsMap;
		std::map<std::string, std::unique_ptr<Model>> _2DModelsMap;
	};

	extern ResourceManager* RESOURCE_MANAGER;
}