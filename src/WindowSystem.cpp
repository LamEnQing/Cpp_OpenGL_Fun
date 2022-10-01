#include "WindowSystem.h"
#include "Exceptions.h"

namespace OpenGLFun {
	WindowSystem* WINDOW_SYSTEM;

	WindowSystem::WindowSystem() : ISystem(), mWindow(nullptr) {
		if (WINDOW_SYSTEM != nullptr)
			throw SimpleException("WindowSystem already created!");
		WINDOW_SYSTEM = this;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(GetWindowWidth(), GetWindowHeight(), "OpenGL_Fun", NULL, NULL);
		if (mWindow == nullptr) {
			throw SimpleException("Failed to create GLFW window");
		}
		glfwMakeContextCurrent(mWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			throw SimpleException("Failed to initialize GLAD");
		}

		glViewport(0, 0, GetWindowWidth(), GetWindowHeight());
	}

	WindowSystem::~WindowSystem() {
		glfwTerminate();
	}

	void WindowSystem::Update(float const&) {
		glfwPollEvents();
	}

	int WindowSystem::GetWindowWidth() { return 1280; }
	int WindowSystem::GetWindowHeight() { return 720; }
}