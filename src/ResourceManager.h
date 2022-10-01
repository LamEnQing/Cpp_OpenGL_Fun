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

		Model* LoadModel(std::string modelFilepath);
		void UnloadModels();
		Model* GetModel(std::string modelFilepath);
	private:
		std::map<std::string, Texture*> _texturesDataMap;
		std::map<std::string, Model*> _modelsDataMap;
	};

	extern ResourceManager* RESOURCE_MANAGER;
}