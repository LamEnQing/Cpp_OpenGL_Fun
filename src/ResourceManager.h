#pragma once
#include "pch.h"

#include "EntityManager.h"
#include "Model.h"
#include "Texture.h"

namespace OpenGLFun {
	class ResourceManager {
	public:

		ResourceManager();
		~ResourceManager();

		Texture* LoadTexture(std::string textureFilepath);
		void UnloadTextures();
		Texture* GetTexture(std::string textureFilepath);

		Mesh* LoadMesh(std::string modelFilepath);

		Model* LoadModel(EntityId const& entityId, std::string modelFilepath);
		void UnloadModels();
		Model* GetModel(EntityId const& entityId);
	private:
		std::map<std::string, std::shared_ptr<Texture>> _texturesDataMap;
		std::map<EntityId, std::shared_ptr<Model>> _modelsDataMap;
	};

	extern ResourceManager* RESOURCE_MANAGER;
}