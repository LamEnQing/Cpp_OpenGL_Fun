#pragma once
#include "pch.h"
#include "Texture.h"

namespace OpenGLFun {
	class ResourceManager {
	public:
		//std::map<std::string, Texture*> mTexturesDataMap;

		ResourceManager();
		~ResourceManager();

		Texture* LoadTexture(std::string textureFilepath);
		void UnloadTextures();
		Texture* GetTexture(std::string textureFilepath);
	private:
		std::map<std::string, Texture*> _texturesDataMap;
	};

	extern ResourceManager* RESOURCE_MANAGER;
}