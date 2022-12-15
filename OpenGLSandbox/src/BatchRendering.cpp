// From: https://www.youtube.com/watch?v=Th4huqR77rI

#include "BatchRendering.h"

#include <imgui/imgui.h>

#include "Texture.h"

BatchRendering::BatchRendering() {
	this->sinonSqTex = OpenGLSandbox::LoadTexture("assets/textures/sinon_square.png");
	this->gwenStacyTex = OpenGLSandbox::LoadTexture("assets/textures/gwen_stacy.png");

	// Create a vertWrap array (VAO)
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Create Vertex Buffer (VBO) and populate it with data from m_vertices
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->maxQuad * 4 * sizeof(OpenGLSandbox::Vertex), nullptr, GL_DYNAMIC_DRAW); // 4 m_vertices per quad

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, texCoords)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, texID)));

	// Create Indices Buffer (EBO)
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->maxQuad * 6 * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW); // A quad is made up of 2 triangles. A triangle is made up of 3 m_vertices. So 2 * 3 = 6 m_vertices to draw a single quad

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
	glBindVertexArray(0); // unbind VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
}

BatchRendering::~BatchRendering() {
	GLuint textures[2] = { sinonSqTex, gwenStacyTex };
	glDeleteTextures(2, textures);

	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ibo);
}

void BatchRendering::Draw(OpenGLSandbox::ShaderProgram& shdrPgm) {
	//ImGui::ShowDemoWindow(NULL);

	ImGui::Begin("BatchRendering", NULL, ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::Button("Add Quad") && quadCount < maxQuad) {
		this->meshData.reserve(1); // pre-allocate space for 4 m_vertices

		float texId = quadCount % 2 == 0 ? 0.0f : 1.0f;
		float posX = -0.5, posY = -0.5f;
		float quadSize = 1.0f, scale = 100.0f;
		this->meshData.emplace_back(OpenGLSandbox::Mesh(std::vector<OpenGLSandbox::VertexWrapper>{ 
			{ posX, posY, 0.0f, OpenGLSandbox::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, texId) },
			{ posX + quadSize, posY, 0.0f, OpenGLSandbox::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, texId) },
			{ posX + quadSize, posY + quadSize, 0.0f, OpenGLSandbox::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, texId) },
			{ posX, posY + quadSize, 0.0f, OpenGLSandbox::Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, texId) }
		}, { 0.0f, 0.0f }, { scale, scale }, 0.0f));

		this->indexData.reserve(6); // pre-allocate space for 6 indices, remember, 3 indices per triangle and 2 triangle per quad.

		// index goes like this: 0,1,2, 2,3,0
		uint32_t quadStart = (uint32_t)this->quadCount * (uint32_t)4;

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
			if (ImGui::Selectable((std::string("Quad #") + std::to_string(i+1)).c_str(), ((int)i) == selectedQuad)) {
				selectedQuad = (int)i;

				OpenGLSandbox::Mesh& mesh = this->meshData.at(static_cast<size_t>(selectedQuad));
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
			OpenGLSandbox::Mesh& mesh = this->meshData.at(static_cast<size_t>(selectedQuad));

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
		std::vector<OpenGLSandbox::Vertex> vertices;
		vertices.reserve(meshData.size() * 4);

		for (auto meshIt = meshData.begin(); meshIt != meshData.end(); meshIt++) {
			for (auto vertexIt = meshIt->m_vertices.begin(); vertexIt != meshIt->m_vertices.end(); vertexIt++)
				vertices.push_back(vertexIt->vertex);
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(OpenGLSandbox::Vertex), &vertices.front());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexData.size() * sizeof(int), &indexData.front());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		hasVertexDataChanged = false;

		std::cout << "Submitting mesh data" << std::endl;
	}

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shdrPgm.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->sinonSqTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->gwenStacyTex);

	auto texLoc = glGetUniformLocation(shdrPgm.mProgramId, "myTextures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(texLoc, 2, samplers);

	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexData.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}