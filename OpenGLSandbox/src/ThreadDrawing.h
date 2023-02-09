/*
Created by Lam En Qing.

Description:
	ThreadDrawing attempts to imitate a level loading screen by delegating the level data load portion onto a thread.

	From this thread, the progress bar of the level loading is incremented.
	To stimulate time passing upon loading each portion of a level data, the progress bar is segmented into 20 "steps"; 100% / 5% gain = 20 steps.

References:
	std::thread
		https://cplusplus.com/reference/thread/thread/thread/
*/
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <thread>
#include <vector>

#include "GlobalDeclared.h"

namespace OpenGLSandbox {
	class ThreadDrawing {
	private:
		std::vector<std::thread> threads_;
		bool wasThreadCreated{ false }, stopThreads{ false };

		float progressBar_{ 0.0f };
		float const maxProgressBar_{ 100.0f };
		double lastTime_{ 0.0f }, timeElapsed_{ 0.0f };

		GLuint vao_{ uint_max }, vbo_{ uint_max }, ibo_{ uint_max };

		void DelegateLoading();
		void DrawShape(float posX, float posY, float width, float height, glm::vec3 color);
		void ImGuiControls();
	public:
		ThreadDrawing();
		~ThreadDrawing();

		void Draw();
	};
}