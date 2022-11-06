#include "Configuration.h"
#include "LevelManager.h"
#include "ModelComponent.h"
#include "ResourceManager.h"
#include "FileOps.h"

namespace OpenGLFun {	
	void Configuration::LoadConfig() {
		std::string configFilepath = ".\\data\\config.json";
		rapidjson::Document document;
		if (document.Parse(Serializer::GetFileContents(configFilepath.c_str()).c_str()).HasParseError()) {
			throw SimpleException(configFilepath + " has an incorrect JSON format");
		}

		if (!document.IsObject())
			throw SimpleException(configFilepath + " must start with a JSON object");

		if (!document.HasMember("window_width") || !document["window_width"].IsInt())
			throw JsonReadException(configFilepath, "window_width", "integer");
		_windowWidth = document["window_width"].GetInt();

		if (!document.HasMember("window_height") || !document["window_height"].IsInt())
			throw JsonReadException(configFilepath, "window_height", "integer");
		_windowHeight = document["window_height"].GetInt();

		if (!document.HasMember("window_title") || !document["window_title"].IsString())
			throw JsonReadException(configFilepath, "window_title", "string");
		_windowTitle = document["window_title"].GetString();

		if (!document.HasMember("level_on_startup") || !document["level_on_startup"].IsString())
			throw JsonReadException(configFilepath, "level_on_startup", "string");
		LEVEL_MANAGER->mCurrentLevel = document["level_on_startup"].GetString();

		if (!document.HasMember("default_model") || !document["default_model"].IsObject())
			throw JsonReadException(configFilepath, "default_model", "JSON object");
		const rapidjson::Value& modelObj = document["default_model"];
		
		if (!modelObj.HasMember("model_type") || !modelObj["model_type"].IsString())
			throw JsonReadException(configFilepath, "default_model", "model_type", "JSON object");
		_defaultModelType = ModelComponent::ParseModelTypeString(modelObj["model_type"].GetString());

		if (!modelObj.HasMember("model") || !modelObj["model"].IsString())
			throw JsonReadException(configFilepath, "default_model", "model_type", "string");
		_defaultModelFilepath = modelObj["model"].GetString();
	}

	void Configuration::ReloadConfig() {
		std::string configFilepath = ".\\data\\config.json";
		rapidjson::Document document;
		if (document.Parse(Serializer::GetFileContents(configFilepath.c_str()).c_str()).HasParseError()) {
			throw SimpleException(configFilepath + " has an incorrect JSON format");
		}

		if (!document.IsObject())
			throw SimpleException(configFilepath + " must start with a JSON object");

		if (!document.HasMember("default_model") || !document["default_model"].IsObject())
			throw JsonReadException(configFilepath, "default_model", "JSON object");
		const rapidjson::Value& modelObj = document["default_model"];

		if (!modelObj.HasMember("model_type") || !modelObj["model_type"].IsString())
			throw JsonReadException(configFilepath, "default_model", "model_type", "JSON object");
		_defaultModelType = ModelComponent::ParseModelTypeString(modelObj["model_type"].GetString());

		if (!modelObj.HasMember("model") || !modelObj["model"].IsString())
			throw JsonReadException(configFilepath, "default_model", "model_type", "string");
		_defaultModelFilepath = modelObj["model"].GetString();

		if (_defaultModelType == ModelType::TwoD)
			RESOURCE_MANAGER->Load2DModel(_defaultModelFilepath);
	}

	const int& Configuration::GetWindowWidth() const {
		return _windowWidth;
	}

	const int& Configuration::GetWindowHeight() const {
		return _windowHeight;
	}

	const std::string& Configuration::GetWindowTitle() const {
		return _windowTitle;
	}

	const ModelType& Configuration::GetDefaultModelType() const {
		return _defaultModelType;
	}

	const std::string& Configuration::GetDefaultModelFilepath() const {
		return _defaultModelFilepath;
	}
}