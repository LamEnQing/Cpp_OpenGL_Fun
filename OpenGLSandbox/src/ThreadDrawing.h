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
#include <string>
#include <thread>
#include <vector>

#include "GlobalDeclared.h"
#include "ShaderProgram.h"

namespace OpenGLSandbox {
	class ThreadDrawing {
	private:
		struct MyVertex {
			float position[2];
			float uv[2];
		};

		// The status of the level loading process
		enum class LevelLoadStatus {
			Nothing, // Default, no level load going on right now
			Loading, // There is a level load currently
			Done, // The level load process has finished, the game/engine needs to know that!

			Interrupted
		};

		std::thread thdLevelLoad_;
		bool wasThreadCreated{ false };

		ShaderProgram texShdrPgm_;
		std::vector<std::string> reqTextures_;
		std::vector<GLuint> textures_;

		GLuint vao_{ uint_max }, vbo_{ uint_max }, ibo_{ uint_max };
		GLuint textured_vao_{ uint_max }, textured_vbo_{ uint_max }, textured_ibo_{ uint_max };

		//std::vector<std::exception> threadExceptions_; // exceptions thrown by a thread
		std::exception_ptr threadException_{ nullptr }; // Reference: https://stackoverflow.com/questions/233127/how-can-i-propagate-exceptions-between-threads
		
		LevelLoadStatus levelLoadStatus_{LevelLoadStatus::Nothing};

		float progressBar_{ 0.0f }, progressStep{ 20.0f };
		float const maxProgressBar_{ 100.0f };
		double lastTime_{ 0.0f }, timeElapsed_{ 0.0f };

		void DelegateLoading(std::string const& msg);
		void DrawShape(float posX, float posY, float width, float height, glm::vec3 color);
		void DrawTexturedShape(float posX, float posY, float width, float height, GLuint textureId);

		void ImGuiControls();
	public:
		ThreadDrawing();
		~ThreadDrawing();

		void Draw();
	};
}