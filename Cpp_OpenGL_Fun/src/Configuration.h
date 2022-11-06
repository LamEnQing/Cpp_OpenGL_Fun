#pragma once
#include "pch.h"

namespace OpenGLFun {
	// One place, read config values, put them into variables and getters
	// No need create pointer or instance of the class, just declare it as Configuration config; or smth like that, to allocate memory on the stack
	class Configuration {
	public:
		// Loads the configuration JSON file
		void LoadConfig();
		void ReloadConfig();

		const int& GetWindowWidth() const;
		const int& GetWindowHeight() const;
		const std::string& GetWindowTitle() const;
		
		const ModelType& GetDefaultModelType() const;
		const std::string& GetDefaultModelFilepath() const;
	private:
		int _windowWidth, _windowHeight;
		std::string _windowTitle;

		ModelType _defaultModelType{ ModelType::TwoD };
		std::string _defaultModelFilepath;
	};

}