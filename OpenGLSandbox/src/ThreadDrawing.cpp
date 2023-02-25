/*
Created by Lam En Qing.

Description:
	ThreadDrawing attempts to stimulate a level loading screen in a game by delegating the level data loading to a thread.

	From this thread, the progress bar of the level loading is incremented.
	To stimulate time passing upon loading each portion of a level data, the progress bar is segmented into 20 "steps"; 100% / 5% gain = 20 steps.

	The main thread, that is, the running code of the class, i.e. Draw(), continues running unblocked by the thread's code.

References:
	std::thread
		https://cplusplus.com/reference/thread/thread/thread/
*/
#include "ThreadDrawing.h"

#include "Main.h"
#include "Texture.h"
#include "Vertex.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include <vector>

namespace OpenGLSandbox {
	ThreadDrawing::ThreadDrawing() {
		texShdrPgm_.CompileAndLink("ortho_test");

		std::vector<VertexPosColor> vertices;
		vertices.reserve(4);

		vertices.emplace_back(-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f);
		vertices.emplace_back(0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f);
		vertices.emplace_back(0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);
		vertices.emplace_back(-0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);

		VertexIndexType indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &this->vao_);
		glBindVertexArray(this->vao_);

		glGenBuffers(1, &this->vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPosColor), &vertices.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPosColor), (void*)(offsetof(VertexPosColor, color)));

		glGenBuffers(1, &this->ibo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		std::vector<MyVertex> myVertices;
		myVertices.reserve(4);

		myVertices.push_back({ -0.5f, -0.5f, 0.0f, 0.0f });
		myVertices.push_back({ 0.5f, -0.5f, 1.0f, 0.0f });
		myVertices.push_back({ 0.5f, 0.5f, 1.0f, 1.0f });
		myVertices.push_back({ -0.5f, 0.5f, 0.0f, 1.0f });

		glGenVertexArrays(1, &this->textured_vao_);
		glBindVertexArray(this->textured_vao_);

		glGenBuffers(1, &this->textured_vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, this->textured_vbo_);
		glBufferData(GL_ARRAY_BUFFER, myVertices.size() * sizeof(MyVertex), &myVertices.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(offsetof(MyVertex, uv)));

		glGenBuffers(1, &this->textured_ibo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->textured_ibo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		lastTime_ = timeElapsed_ = glfwGetTime();
	}

	ThreadDrawing::~ThreadDrawing() {
		if (vao_ != UINT32_MAX)
			glDeleteVertexArrays(1, &vao_);
		if (vbo_ != UINT32_MAX)
			glDeleteBuffers(1, &vbo_);
		if (ibo_ != UINT32_MAX)
			glDeleteBuffers(1, &ibo_);

		if (textured_vao_ != UINT32_MAX)
			glDeleteVertexArrays(1, &textured_vao_);
		if (textured_vbo_ != UINT32_MAX)
			glDeleteBuffers(1, &textured_vbo_);
		if (textured_ibo_ != UINT32_MAX)
			glDeleteBuffers(1, &textured_ibo_);

		if (thdLevelLoad_.joinable()) {
			progressBar_ = maxProgressBar_;
			thdLevelLoad_.join();
		}
	}

	void ThreadDrawing::DelegateLoading(std::string const& msg) {
		std::cout << "DelegateLoading: msg is " << msg << std::endl;

		static std::string names[] = {
			"gold_block.png", "gwen_stacy.png", "moon.png", "sinon_square.png", "sun.png"
		};

		try {
			textures_.clear();
			while (progressBar_ < maxProgressBar_) {
				double currTime = glfwGetTime();

				double timeDiff = currTime - timeElapsed_;

				if (timeDiff >= 1.5) {
					progressBar_ += progressStep;

					int idx = static_cast<int>(progressBar_ / progressStep) - 1;
					reqTextures_.push_back(names[idx]);

					if (progressBar_ >= 50.0f) { // Say we encountered an exception
						//throw std::exception("DelegateLoading threw an exception!");
					}

					timeElapsed_ = glfwGetTime();
				}
				//std::cout << "ThreadDrawing::DelegateLoading():" << progressBar_ << " | " << timeDiff << std::endl;
			}
			levelLoadStatus_ = LevelLoadStatus::Done;
		}
		catch (...) {
			threadException_ = std::current_exception();
		}
	}

	void ThreadDrawing::Draw() {
		if (!wasThreadCreated) {
			wasThreadCreated = true;
			levelLoadStatus_ = LevelLoadStatus::Loading;
			thdLevelLoad_ = std::thread(&ThreadDrawing::DelegateLoading, this, "Hi there");
		}

		if (reqTextures_.size() > 0) {
			for (const std::string& texName : reqTextures_) {
				textures_.push_back(LoadTexture(texName));
			}
			reqTextures_.clear();
		}

		// Handle thrown exceptions
		if (threadException_) {
			if (thdLevelLoad_.joinable()) {
				thdLevelLoad_.join();
			}
			try {
				std::rethrow_exception(threadException_);
			}
			catch (std::exception const& e) {
				levelLoadStatus_ = LevelLoadStatus::Interrupted;
				std::cout << "Thread had an exception:" << e.what() << std::endl;
			}
			threadException_ = nullptr; // since thread's exception was thrown, set it to null
		}

		//std::cout << "ThreadDrawing::Draw():" << progressBar_ << std::endl;

		posColorShdrPgm.use();

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float containerWidth = 500.0f, containerHeight = 100.0f;
		float maxProgBarWidth = 460.0f, progBarHeight = 60.0f;

		DrawShape(0.f, 0.f, containerWidth, containerHeight, glm::vec3(0.0f));

		float progBarWidthActual = maxProgBarWidth * progressBar_ / maxProgressBar_;
		DrawShape(-(maxProgBarWidth - progBarWidthActual)/2.0f, 0.f, progBarWidthActual, progBarHeight, glm::vec3(1.0f));

		float statusPosX = 300.0f, statusPosY = 200.0f, statusLength = 100.0f;
		glm::vec3 statusColor(0.0f);
		DrawShape(statusPosX, statusPosY, statusLength, statusLength, statusColor); // draw outline

		switch (levelLoadStatus_) {
			case LevelLoadStatus::Loading: {
				statusColor = glm::vec3(1.0f, 1.0f, 0.0f);
				break;
			}
			case LevelLoadStatus::Done: {
				statusColor = glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			}
			case LevelLoadStatus::Interrupted: {
				statusColor = glm::vec3(1.0f, 0.0f, 0.0f);
				break;
			}
		}
		DrawShape(statusPosX, statusPosY, statusLength - 20.0f, statusLength - 20.0f, statusColor); // draw status box

		texShdrPgm_.use();
		int i = 0;
		float width = SCREEN_WIDTH / progressStep;
		for (auto texId : this->textures_) {
			DrawTexturedShape(-SCREEN_WIDTH + i * width, SCREEN_HEIGHT - 10.f, width, 30.0f, texId);
			i++;
		}
	}

	void ThreadDrawing::DrawShape(float posX, float posY, float width, float height, glm::vec3 color) {
		float scrWidth = static_cast<float>(SCREEN_WIDTH), scrHeight = static_cast<float>(SCREEN_HEIGHT);

		glm::mat4 transforms(1.0f);
		transforms = glm::translate(transforms, glm::vec3(posX / scrWidth, posY / scrHeight, 0.0f));
		transforms = glm::scale(transforms, glm::vec3(width / scrWidth, height / scrHeight, 1.0f));

		auto transformsLoc = glGetUniformLocation(currShdrPgm, "uTransformation");
		glUniformMatrix4fv(transformsLoc, 1, GL_FALSE, glm::value_ptr(transforms));

		auto tintColorLoc = glGetUniformLocation(currShdrPgm, "uTintColor");
		glUniform3fv(tintColorLoc, 1, glm::value_ptr(color));

		glBindVertexArray(this->vao_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void ThreadDrawing::DrawTexturedShape(float posX, float posY, float width, float height, GLuint textureId) {
		float scrWidth = static_cast<float>(SCREEN_WIDTH), scrHeight = static_cast<float>(SCREEN_HEIGHT);

		glm::mat4 transforms(1.0f);
		transforms = glm::translate(transforms, glm::vec3(posX / scrWidth, posY / scrHeight, 0.0f));
		transforms = glm::scale(transforms, glm::vec3(width / scrWidth, height / scrHeight, 1.0f));

		auto transformsLoc = glGetUniformLocation(currShdrPgm, "uTransformation");
		glUniformMatrix4fv(transformsLoc, 1, GL_FALSE, glm::value_ptr(transforms));

		glBindTexture(GL_TEXTURE_2D, textureId);

		glBindVertexArray(this->textured_vao_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}