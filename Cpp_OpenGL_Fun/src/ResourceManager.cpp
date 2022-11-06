#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "ResourceManager.h"
#include "Shape.h"
#include "GraphicSystem.h"
#include "Serializer.h"

namespace OpenGLFun {
	ShapeManager* SHAPE_MANAGER = nullptr;

	Model* ParseModel(const std::string& modelFilepath);

	ResourceManager::ResourceManager() : mTexturesDataMap{}, _3DModelsMap{} {
		if (RESOURCE_MANAGER != nullptr)
			throw SimpleException("Resource manager has already been created!");

		RESOURCE_MANAGER = this;

		SHAPE_MANAGER = new ShapeManager();

		stbi_set_flip_vertically_on_load(1);
	}
	ResourceManager::~ResourceManager() {
		delete SHAPE_MANAGER;
		UnloadTextures();
		UnloadModels();
	}

	Texture* ResourceManager::LoadTexture(std::string textureFilepath) {
		std::string texturesDirPath = std::string("assets/textures/");

		if (mTexturesDataMap.find(textureFilepath) != mTexturesDataMap.end()) {
			std::cout << textureFilepath + " was already loaded, getting you the existing texture instead!\n";
			return mTexturesDataMap.at(textureFilepath).get();
		}

		Texture* texture = new Texture();
		texture->imgData = stbi_load((texturesDirPath + textureFilepath).c_str(), &texture->imgWidth, &texture->imgHeight, &texture->imgChannels, 0);

		if (!texture->imgData) {
			delete texture;
			throw SimpleException(std::string("Failed to load texture ") + texturesDirPath + textureFilepath);
		}

		std::cout << "Loading texture " << textureFilepath << '\n';

		mTexturesDataMap.insert({ textureFilepath, std::shared_ptr<Texture>(texture) });

		GRAPHICS_SYSTEM->CreateGLTexture(texture);

		// texture variable will be out of scope, so instead we return the texture from the map
		return mTexturesDataMap.at(textureFilepath).get();
	}

	Texture* ResourceManager::GetTexture(std::string textureFilepath) {
		if (mTexturesDataMap.find(textureFilepath) == mTexturesDataMap.end()) {
			throw SimpleException(std::string("Could not find ") + textureFilepath + " in texture database");
		}

		return mTexturesDataMap.at(textureFilepath).get();
	}

	void ResourceManager::UnloadTextures() {
		for (auto textureIt = mTexturesDataMap.rbegin(); textureIt != mTexturesDataMap.rend(); textureIt++) {
			delete textureIt->second->imgData; // need to remove this img data
			GRAPHICS_SYSTEM->DeleteGLTexture(textureIt->second->mGLTextureId);
		}
		mTexturesDataMap.clear();
	}

	Mesh* ResourceManager::LoadMesh(std::string meshFilepath) {
		std::string meshDirPath = std::string("assets/models/meshes/");

		if (!(Serializer::DoesFilenameEndWith(meshFilepath, ".json") || Serializer::DoesFilenameEndWith(meshFilepath, ".obj"))) {
			throw SimpleException(meshDirPath + meshFilepath + " must either be in JSON or Wavefront (.obj) format");
		}

		if (Serializer::DoesFilenameEndWith(meshFilepath, ".json")) {
			std::string json_from_file = Serializer::GetFileContents((meshDirPath + meshFilepath).c_str());

			rapidjson::Document document;
			if (document.Parse(json_from_file.c_str()).HasParseError()) {
				// I put filepath into std::string, so that I can append new strings or char pointers via add operator
				throw SimpleException(meshDirPath + meshFilepath + " has an incorrect JSON format");
			}

			// the root must be a JSON object
			if (!document.IsObject())
				throw SimpleException(meshDirPath + meshFilepath + " must start with a JSON object");

			std::cout << "Loading mesh " << meshFilepath << '\n';
			Mesh* mesh = new Mesh();

			try {
				mesh->DeserializeJson(document.GetObject());
			}
			catch (std::exception const& e) { // catch all parse errors, so that we can include the filename in the error too!
				delete mesh;
				throw SimpleException(std::string("Failed to parse ") + meshDirPath + meshFilepath + ", here's the parse error:\n\t" + e.what());
			}

			return mesh;
		}
		else { // Obj deserialization
			Mesh* mesh = new Mesh();
			std::cout << "Loading mesh " << meshFilepath << '\n';

			try {
				mesh->DeserializeObj(meshDirPath + meshFilepath);
			}
			catch (std::exception const& e) {
				delete mesh;
				throw SimpleException(std::string("Failed to parse ") + meshDirPath + meshFilepath + ", here's the parse error:\n\t" + e.what());
			}

			return mesh;
		}
	}

	Model* ResourceManager::Load2DModel(std::string modelFilepath) {
		// If 2D map has the 2D model loaded, then no need load, just return instance
		if (_2DModelsMap.find(modelFilepath) != _2DModelsMap.end()) {
			std::cout << "2D model " << modelFilepath << " is already loaded" << std::endl;
			return _2DModelsMap.at(modelFilepath).get();
		}

		std::cout << "Loading 2D model " << modelFilepath << std::endl;
		Model* model = ParseModel(modelFilepath);

		_2DModelsMap.insert({ modelFilepath, std::unique_ptr<Model>(model) });

		return _2DModelsMap.at(modelFilepath).get();
	}

	std::shared_ptr<Model> ResourceManager::Load3DModel(EntityId const& entityId, std::string modelFilepath) {
		std::cout << "Loading 3D model " << modelFilepath << " for " << entityId << std::endl;
		Model* model = ParseModel(modelFilepath);

		_3DModelsMap.insert({ entityId, std::shared_ptr<Model>(model) });

		return _3DModelsMap.at(entityId);
	}

	Model* ResourceManager::Get2DModel(std::string const& modelFilepath) {
		if (_2DModelsMap.find(modelFilepath) == _2DModelsMap.end()) {
			throw SimpleException(std::string("Could not find 2D model ") + modelFilepath + " in model database");
		}

		return _2DModelsMap.at(modelFilepath).get();
	}

	std::shared_ptr<Model>& ResourceManager::Get3DModel(EntityId const& entityId) {
		if (_3DModelsMap.find(entityId) == _3DModelsMap.end()) {
			throw SimpleException(std::string("Could not find entity ") + std::to_string(entityId) + "'s 3D model in model database");
		}

		return _3DModelsMap.at(entityId);
	}

	void ResourceManager::UnloadModels() {
		for (auto modelIt = _3DModelsMap.rbegin(); modelIt != _3DModelsMap.rend(); modelIt++) {
			modelIt->second->Destroy();
		}
		_3DModelsMap.clear();

		for (auto modelIt = _2DModelsMap.rbegin(); modelIt != _2DModelsMap.rend(); modelIt++) {
			modelIt->second->Destroy();
		}
		_2DModelsMap.clear();
	}

	Model* ParseModel(const std::string& modelFilepath) {
		std::string modelDirPath = std::string("assets/models/");

		if (!Serializer::DoesFilenameEndWith(modelFilepath, ".json")) {
			throw SimpleException(modelDirPath + modelFilepath + " must be in JSON format");
		}

		std::string json_from_file = Serializer::GetFileContents((modelDirPath + modelFilepath).c_str());

		rapidjson::Document document;
		if (document.Parse(json_from_file.c_str()).HasParseError()) {
			// I put filepath into std::string, so that I can append new strings or char pointers via add operator
			throw SimpleException(modelDirPath + modelFilepath + " has an incorrect JSON format");
		}

		// the root must be a JSON object
		if (!document.IsArray() || document.Size() <= 0)
			throw SimpleException(modelDirPath + modelFilepath + " must start with a JSON array containing at least 1 JSON object");

		std::cout << "Parsing model " << modelFilepath << '\n';
		Model* model = new Model();

		try {
			for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
				rapidjson::Value& meshJson = document[i];

				if (!meshJson.IsObject())
					throw JsonReadException(modelDirPath + modelFilepath, std::string("for index ") + std::to_string(i), "JSON object");

				if (!meshJson.HasMember("part_name") || !meshJson["part_name"].IsString())
					throw JsonReadException(modelDirPath + modelFilepath, std::string("for index ") + std::to_string(i), "part_name", "string");

				if (!meshJson.HasMember("mesh") || !meshJson["mesh"].IsString())
					throw JsonReadException(modelDirPath + modelFilepath, std::string("for index ") + std::to_string(i), "mesh", "string");

				Vec3f offsetVec = Vec3f(0.0f);
				if (meshJson.HasMember("offset")) {
					if (!meshJson["offset"].IsArray())
						throw JsonReadException(modelDirPath + modelFilepath, std::string("for index ") + std::to_string(i), "offset", "array of 3 elements");

					const rapidjson::Value& offsetArr = meshJson["offset"];
					for (int j = 0; j < 3; j++) {
						if (!offsetArr[j].IsNumber())
							throw JsonReadException(modelDirPath + modelFilepath, std::string("for index ") + std::to_string(j), std::string("offset[") + std::to_string(j) + "]", "number");

						offsetVec[j] = offsetArr[j].GetFloat();
					}
				}

				model->AddMesh(meshJson["part_name"].GetString(), std::shared_ptr<Mesh>(RESOURCE_MANAGER->LoadMesh(meshJson["mesh"].GetString())->SetOffset(offsetVec)));
			}
		}
		catch (std::exception const& e) { // catch all parse errors, so that we can include the filename in the error too!
			delete model;
			throw SimpleException(std::string("Failed to parse ") + modelDirPath + modelFilepath + ", here's the parse error:\n\t" + e.what());
		}

		return model;
	}
}