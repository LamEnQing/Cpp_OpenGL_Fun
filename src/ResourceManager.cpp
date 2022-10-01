#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "ResourceManager.h"
#include "GraphicSystem.h"

namespace OpenGLFun {

	ResourceManager::ResourceManager() : _texturesDataMap{} {
		if (RESOURCE_MANAGER != nullptr)
			throw SimpleException("Resource manager has already been created!");

		RESOURCE_MANAGER = this;
	}
	ResourceManager::~ResourceManager() {
		UnloadTextures();
	}

	Texture* ResourceManager::LoadTexture(std::string textureFilepath) {
		if (_texturesDataMap.find(textureFilepath) != _texturesDataMap.end()) {
			std::cout << textureFilepath + " was already loaded, getting you the existing texture instead!\n";
			return _texturesDataMap.at(textureFilepath);
		}

		Texture* texture = new Texture();
		texture->imgData = stbi_load(textureFilepath.c_str(), &texture->imgWidth, &texture->imgHeight, &texture->imgChannels, 0);

		if (!texture->imgData) {
			delete texture;
			throw SimpleException(std::string("Failed to load texture ") + textureFilepath);
		}

		std::cout << "Loading " << textureFilepath << '\n';

		_texturesDataMap.insert({ textureFilepath, texture });

		GRAPHICS_SYSTEM->CreateGLTexture(texture);

		// texture variable will be out of scope, so instead we return the texture from the map
		return _texturesDataMap.at(textureFilepath);
	}

	Texture* ResourceManager::GetTexture(std::string textureFilepath) {
		if (_texturesDataMap.find(textureFilepath) == _texturesDataMap.end()) {
			throw SimpleException(std::string("Could not find ") + textureFilepath + " in texture database");
		}

		return _texturesDataMap.at(textureFilepath);
	}

	void ResourceManager::UnloadTextures() {
		for (auto textureIt = _texturesDataMap.rbegin(); textureIt != _texturesDataMap.rend(); textureIt++) {
			delete textureIt->second->imgData; // need to remove this img data
			GRAPHICS_SYSTEM->DeleteGLTexture(textureIt->second->mGLTextureId);
			delete textureIt->second;
		}
		_texturesDataMap.clear();
	}
}