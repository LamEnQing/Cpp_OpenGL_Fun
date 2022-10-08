#pragma once
#include "pch.h"
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

		std::shared_ptr<Mesh> LoadMesh(std::string modelFilepath);
		void UnloadMesh();
		std::shared_ptr<Mesh> GetMesh(std::string modelFilepath);

		Model* LoadModel(std::string modelFilepath);
		void UnloadModels();
		Model* GetModel(std::string modelFilepath);
	private:
		std::map<std::string, std::shared_ptr<Texture>> _texturesDataMap;
		std::map<std::string, std::shared_ptr<Mesh>> _meshesDataMap;
		std::map<std::string, std::shared_ptr<Model>> _modelsDataMap;
	};

	extern ResourceManager* RESOURCE_MANAGER;
}