#pragma once
#include "pch.h"
#include "System.h"

namespace OpenGLFun {
	class WindowSystem : public ISystem {
	public:
		GLFWwindow* mWindow;
		WindowSystem();
		~WindowSystem();
		virtual void Update(float const&) override;

		int GetWindowWidth();
		int GetWindowHeight();
	private:
		int _windowWidth;
		int _windowHeight;
	};

	extern WindowSystem* WINDOW_SYSTEM;
}