#pragma once
#include <GLFW/glfw3.h>

namespace OpenGLFun {
namespace Game {
	void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void Input(GLFWwindow* window);

	void Init(GLFWwindow* window);
	void Update(void);
	void Render(void);

	const float& GetDeltaTime();
	const float& GetLastFrame();
	void SetDeltaTime(const float& time);
	void SetLastFrame(const float& frame);
}
}