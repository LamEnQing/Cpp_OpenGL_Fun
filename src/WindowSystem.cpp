#include "WindowSystem.h"
#include "Exceptions.h"
#include "Serializer.h"
#include "LevelManager.h"
#include "InputSystem.h"

namespace OpenGLFun {
	static std::string WINDOW_TITLE{};

	WindowSystem* WINDOW_SYSTEM;

	WindowSystem::WindowSystem() : ISystem(), mWindow(nullptr), _windowWidth(0), _windowHeight(0) {
		if (WINDOW_SYSTEM != nullptr)
			throw SimpleException("WindowSystem already created!");
		WINDOW_SYSTEM = this;

		// Read JSON data
		std::string configFilepath = "data\\config.json";
		rapidjson::Document document;
		if (document.Parse(Serializer::GetFileContents(configFilepath.c_str()).c_str()).HasParseError()) {
			throw SimpleException(configFilepath + " has an incorrect JSON format");
		}

		if (!document.IsObject())
			throw SimpleException(configFilepath + " must start with a JSON object");
		if (!document.HasMember("window_width") || !document["window_width"].IsInt())
			throw JsonReadException(configFilepath, "window_width", "integer");
		if (!document.HasMember("window_height") || !document["window_height"].IsInt())
			throw JsonReadException(configFilepath, "window_height", "integer");
		if (!document.HasMember("window_title") || !document["window_title"].IsString())
			throw JsonReadException(configFilepath, "window_title", "string");

		_windowWidth = document["window_width"].GetInt();
		_windowHeight = document["window_height"].GetInt();
		WINDOW_TITLE = document["window_title"].GetString();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(GetWindowWidth(), GetWindowHeight(), WINDOW_TITLE.c_str(), NULL, NULL);
		if (mWindow == nullptr) {
			throw SimpleException("Failed to create GLFW window");
		}
		glfwMakeContextCurrent(mWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate(); // exception thrown in construction, so destructor for system won't run
			throw SimpleException("Failed to initialize GLAD");
		}

		if (!document.HasMember("level_on_startup") || !document["level_on_startup"].IsString())
			throw JsonReadException(configFilepath, "level_on_startup", "string");
		LEVEL_MANAGER->mCurrentLevel = document["level_on_startup"].GetString();
	}

	WindowSystem::~WindowSystem() {
		glfwTerminate();
	}

	void WindowSystem::Update(float const&) {
		glfwPollEvents();
	}

	int WindowSystem::GetWindowWidth() { return _windowWidth; }
	int WindowSystem::GetWindowHeight() { return _windowHeight; }
}