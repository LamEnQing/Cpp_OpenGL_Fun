#include "WindowSystem.h"
#include "Engine.h"
#include "Exceptions.h"
#include "LevelManager.h"

namespace OpenGLFun {
	static std::string WINDOW_TITLE{};

	WindowSystem* WINDOW_SYSTEM;

	WindowSystem::WindowSystem() : ISystem(), mWindow(nullptr), _windowWidth(ENGINE->mConfig.GetWindowWidth()), _windowHeight(ENGINE->mConfig.GetWindowHeight()), mFrameWidth(0), mFrameHeight(0) {
		if (WINDOW_SYSTEM != nullptr)
			throw SimpleException("WindowSystem already created!");
		WINDOW_SYSTEM = this;

		WINDOW_TITLE = ENGINE->mConfig.GetWindowTitle();

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
	}

	WindowSystem::~WindowSystem() {
		glfwTerminate();
	}

	void WindowSystem::Update(float const&) {
		glfwPollEvents();

		// update frame buffer size variables every frame
		glfwGetFramebufferSize(mWindow, &mFrameWidth, &mFrameHeight);
	}

	int WindowSystem::GetWindowWidth() { return _windowWidth; }
	int WindowSystem::GetWindowHeight() { return _windowHeight; }
}