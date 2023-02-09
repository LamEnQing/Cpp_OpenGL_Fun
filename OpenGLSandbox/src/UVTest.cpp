#include "UVTest.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <vector>

#include "Texture.h"

namespace OpenGLSandbox {
	UVTest::UVTest() : uvOffset{ 0, 0 }, uvSize{ 1.0f, 1.0f } {
		if (!mainShdr.CompileAndLink("uv_test")) {
			mainShdr.Destroy();
			throw std::exception("Failed to compile 'uv_test' shaders.\n");
		}

		this->myTex = LoadTexture("gold_block.png", &this->imgWidth, &this->imgHeight);
		uvSize[0] = (float)this->imgWidth;
		uvSize[1] = (float)this->imgHeight;

		std::vector<MyVertex> vertices;
		vertices.reserve(4);

		vertices.emplace_back(-0.5f, -0.5f, 0.0f, 1.0f);
		vertices.emplace_back( 0.5f, -0.5f, 1.0f, 1.0f);
		vertices.emplace_back( 0.5f,  0.5f, 1.0f, 0.0f);
		vertices.emplace_back(-0.5f,  0.5f, 0.0f, 0.0f);

		VertexIndexType indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MyVertex), &vertices.front(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(offsetof(MyVertex, uv)));

		glGenBuffers(1, &this->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	UVTest::~UVTest() {}

	void UVTest::Draw() {
		mainShdr.use();

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		#pragma region ImGui
		ImGui::Begin("UV Test");
		ImGui::Text(std::string("Img Width:" + std::to_string(imgWidth)).c_str()); ImGui::SameLine();
		ImGui::Text(std::string("Img Height:" + std::to_string(imgWidth)).c_str());

		ImGui::SliderFloat("UV Tex Width", uvSize, 1.0f, static_cast<float>(imgWidth), "%.0f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("UV Tex Height", uvSize + 1, 1.0f, static_cast<float>(imgHeight), "%.0f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("UV Tex X-Offset", uvOffset, 0.0f, static_cast<float>(imgWidth), "%.0f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderFloat("UV Tex Y-Offset", uvOffset + 1, 0.0f, static_cast<float>(imgHeight), "%.0f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::End();
		#pragma endregion

		//auto uTransformation
		auto texSizeLoc = glGetUniformLocation(currShdrPgm, "uTexSize");
		glUniform2fv(texSizeLoc, 1, glm::value_ptr(glm::vec2(uvSize[0]/(float)imgWidth, uvSize[1]/(float)imgHeight)));

		auto texOffsetLoc = glGetUniformLocation(currShdrPgm, "uTexOffset");
		glUniform2fv(texOffsetLoc, 1, glm::value_ptr(glm::vec2(uvOffset[0] / (float)imgWidth, uvOffset[1] / (float)imgHeight)));

		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myTex);

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}