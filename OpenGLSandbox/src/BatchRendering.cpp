// From: https://www.youtube.com/watch?v=Th4huqR77rI

#include "BatchRendering.h"

#include <imgui/imgui.h>

#include "Texture.h"

namespace OpenGLSandbox {
	BatchRendering::BatchRendering() {
		this->sinonSqTex = OpenGLSandbox::LoadTexture("sinon_square.png");
		this->gwenStacyTex = OpenGLSandbox::LoadTexture("gwen_stacy.png");

		this->CreateBuffers(this->maxQuad * 4 * sizeof(Vertex), this->maxQuad * 6 * sizeof(VertexIndexType));
	}

	BatchRendering::~BatchRendering() {
		GLuint textures[2] = { sinonSqTex, gwenStacyTex };
		glDeleteTextures(2, textures);
	}

	void BatchRendering::Draw(OpenGLSandbox::ShaderProgram& shdrPgm) {
		//ImGui::ShowDemoWindow(NULL);

		ImGui::Begin("BatchRendering", NULL, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::Text((std::string("Buffer updates count:") + std::to_string(this->bufferUpdates)).c_str());

		if (ImGui::Button("Add Quad") && quadCount < maxQuad) {
			this->meshData.reserve(1); // pre-allocate space for 4 m_vertices

			float texId = quadCount % 2 == 0 ? 0.0f : 1.0f;
			float posX = -0.5, posY = -0.5f;
			float quadSize = 1.0f, scale = 100.0f;
			this->meshData.emplace_back(Mesh(std::vector<VertexWrapper>{
				{ posX, posY, 0.0f, Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, texId) },
				{ posX + quadSize, posY, 0.0f, Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, texId) },
				{ posX + quadSize, posY + quadSize, 0.0f, Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, texId) },
				{ posX, posY + quadSize, 0.0f, Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, texId) }
			}, { 0.0f, 0.0f }, { scale, scale }, 0.0f));

			this->indexData.reserve(6); // pre-allocate space for 6 indices, remember, 3 indices per triangle and 2 triangle per quad.

			// index goes like this: 0,1,2, 2,3,0
			VertexIndexType quadStart = (VertexIndexType)this->quadCount * (VertexIndexType)4;

			this->indexData.emplace_back(quadStart);
			this->indexData.emplace_back(quadStart + 1);
			this->indexData.emplace_back(quadStart + 2);

			this->indexData.emplace_back(quadStart + 2);
			this->indexData.emplace_back(quadStart + 3);
			this->indexData.emplace_back(quadStart);

			this->quadCount++; // at the end, increment the quad count

			hasVertexDataChanged = true;
		}

		static float posX = 0, posY = 0;
		static float rotation = 0;
		static float scaleX = 0, scaleY = 0;

		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::BeginListBox("##Quad List")) {
			for (size_t i = 0; i < quadCount; i++) {
				if (ImGui::Selectable((std::string("Quad #") + std::to_string(i + 1)).c_str(), ((int)i) == selectedQuad)) {
					selectedQuad = (int)i;

					Mesh& mesh = this->meshData.at(static_cast<size_t>(selectedQuad));
					posX = mesh.m_position[0];
					posY = mesh.m_position[1];
					rotation = mesh.m_rotation;
					scaleX = mesh.m_scale[0];
					scaleY = mesh.m_scale[1];
				}

				if (selectedQuad == (int)i) {
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndListBox();
		}

		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Quad Property Editor")) {
			if (selectedQuad != -1) {
				Mesh& mesh = this->meshData.at(static_cast<size_t>(selectedQuad));

				ImGui::PushItemWidth(50);

				// Position will set from the bottom left
				ImGui::Text("Position:");
				ImGui::SameLine(); ImGui::Text("X");
				ImGui::SameLine(); ImGui::InputFloat("##posX", &posX, -1);
				ImGui::SameLine(); ImGui::Text("Y");
				ImGui::SameLine(); ImGui::InputFloat("##posY", &posY, -1);

				ImGui::Text("Rotation:");
				ImGui::SameLine(); ImGui::InputFloat("##rot", &rotation, -1);

				ImGui::Text("Scale:");
				ImGui::SameLine(); ImGui::Text("X");
				ImGui::SameLine(); ImGui::InputFloat("##scaleX", &scaleX, -1);
				ImGui::SameLine(); ImGui::Text("Y");
				ImGui::SameLine(); ImGui::InputFloat("##scaleY", &scaleY, -1);

				ImGui::PopItemWidth();

				if (posX != mesh.m_position[0] || posY != mesh.m_position[1] || scaleX != mesh.m_scale[0] || scaleY != mesh.m_scale[1] || rotation != mesh.m_rotation) {
					mesh.SetTransforms(posX, posY, rotation, scaleX, scaleY);

					hasVertexDataChanged = true;
				}
			}
		}

		ImGui::End();

		// Set dynamic vertWrap buffer
		if (hasVertexDataChanged) {
			UpdateBuffers();

			hasVertexDataChanged = false;
		}

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shdrPgm.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->sinonSqTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->gwenStacyTex);

		auto texLoc = glGetUniformLocation(currShdrPgm, "myTextures");
		int samplers[2] = { 0, 1 };
		glUniform1iv(texLoc, 2, samplers);

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexData.size()), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}