#include "OrthoTest.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <vector>

#include "Main.h"
#include "Texture.h"

namespace OpenGLSandbox {
	int fbWidth = SCREEN_WIDTH, fbHeight = SCREEN_HEIGHT;

	void fbSizeCb(GLFWwindow* window, int pixelsWidth, int pixelsHeight);

	OrthoTest::OrthoTest() {
		glfwSetFramebufferSizeCallback(windowPtr, fbSizeCb);

		this->myTex = LoadTexture("background.jpg");

		if (!myShdrPgm.CompileAndLink("ortho_test")) {
			myShdrPgm.Destroy();
			throw std::exception("Failed to compile 'ortho_test' shaders.\n");
		}

		float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		VertexIndexType indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glGenBuffers(1, &this->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OrthoTest::~OrthoTest() {}

	void OrthoTest::ImGuiControls() {
		int oldWidth = fbWidth, oldHeight = fbHeight;

		ImGui::Begin("Ortho Test");

		ImGui::Text((std::string("Original Window Size: ") + std::to_string((int)SCREEN_WIDTH) + ", " + std::to_string((int)SCREEN_HEIGHT)).c_str());

		ImGui::SliderInt("Window Width", &fbWidth, 50, 1920, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Window Height", &fbHeight, 50, 1080, "%d", ImGuiSliderFlags_AlwaysClamp);

		if (oldWidth != fbWidth || oldHeight != fbHeight)
			glfwSetWindowSize(windowPtr, fbWidth, fbHeight);

		ImGui::SliderFloat("Zoom", &zoom, 0.1f, 5.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::End();
	}

	void OrthoTest::Draw() {
		this->ImGuiControls();

		//float aspectRatio = fbWidth / (float)fbHeight;
		glViewport(0, 0, fbWidth, fbHeight);

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);
		//glEnable(GL_DEPTH_TEST);

		myShdrPgm.use();

		glm::mat4 defaultMtx(1.0f);
		glm::mat4 scaleMtx = glm::scale(defaultMtx, glm::vec3(SCREEN_WIDTH / (float)fbWidth, SCREEN_HEIGHT / (float) fbHeight, 0.0f));

		glm::mat4 orthoProj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f) * glm::scale(defaultMtx, glm::vec3(zoom)) * scaleMtx;
		auto transformsLoc = glGetUniformLocation(currShdrPgm, "uTransformation");
		glUniformMatrix4fv(transformsLoc, 1, GL_FALSE, glm::value_ptr(orthoProj));

		glBindTexture(GL_TEXTURE_2D, myTex);

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//glDisable(GL_DEPTH_TEST);
	}

	void fbSizeCb(GLFWwindow* /*window*/, int pixelsWidth, int pixelsHeight) {
		fbWidth = pixelsWidth;
		fbHeight = pixelsHeight;
	}
}