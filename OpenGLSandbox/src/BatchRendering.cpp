// From: https://www.youtube.com/watch?v=Th4huqR77rI

#include "BatchRendering.h"

#include <imgui/imgui.h>

#include "Texture.h"

BatchRendering::BatchRendering() {
	this->sinonSqTex = OpenGLSandbox::LoadTexture("assets/textures/sinon_square.png");
	this->gwenStacyTex = OpenGLSandbox::LoadTexture("assets/textures/gwen_stacy.png");

	// Create a vertex array (VAO)
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	// Create Vertex Buffer (VBO) and populate it with data from vertices
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->maxQuad * 4 * sizeof(OpenGLSandbox::Vertex), nullptr, GL_DYNAMIC_DRAW); // 4 vertices per quad

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, texCoords)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(OpenGLSandbox::Vertex), (void*)(offsetof(OpenGLSandbox::Vertex, texID)));

	// Crate Indices Buffer (EBO)
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->maxQuad * 6 * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW); // A quad is made up of 2 triangles. A triangle is made up of 3 vertices. So 2 * 3 = 6 vertices to draw a single quad

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
		this->vertexData.reserve(4); // pre-allocate space for 4 vertices

		float texId = quadCount % 2 == 0 ? 0.0f : 1.0f;
		this->vertexData.emplace_back(-0.75f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, texId);
		this->vertexData.emplace_back(-0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, texId);
		this->vertexData.emplace_back(-0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, texId);
		this->vertexData.emplace_back(-0.75f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, texId);

		this->indexData.reserve(6); // pre-allocate space for 6 indices, remember, 3 indices per triangle and 2 triangle per quad.

		// index goes like this: 0,1,2, 2,3,0
		uint32_t quadStart = this->quadCount * (uint32_t)4;

		this->indexData.emplace_back(quadStart);
		this->indexData.emplace_back(quadStart + 1);
		this->indexData.emplace_back(quadStart + 2);

		this->indexData.emplace_back(quadStart + 2);
		this->indexData.emplace_back(quadStart + 3);
		this->indexData.emplace_back(quadStart);

		this->quadCount++; // at the end, increment the quad count

		hasVertexDataChanged = true;
	}

	ImGui::SetNextItemWidth(-FLT_MIN);
	if (ImGui::BeginListBox("##Quad List")) {
		for (size_t i = 1; i <= quadCount; i++) {
			ImGui::Selectable((std::string("Quad #") + std::to_string(i)).c_str());
		}

		ImGui::EndListBox();
	}

	ImGui::NewLine();
	if (ImGui::CollapsingHeader("Quad Property Editor")) {
		ImGui::Text("Position:");
		ImGui::SameLine(); ImGui::Text("X");
		ImGui::SameLine(); ImGui::InputFloat("##posx", NULL);
		ImGui::SameLine(); ImGui::Text("Y");
		ImGui::SameLine();

		ImGui::Text("Rotation:");

		ImGui::Text("Scale:");
	}

	ImGui::End();

	// x,y,z, r,g,b,a, u,v, texIndex
	OpenGLSandbox::Vertex vertices[] = {
		{ -0.75f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{ -0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -0.75f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },

		{ 0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.75f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 0.75f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f }
	};

	uint32_t idk_indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	// Set dynamic vertex buffer
	if (hasVertexDataChanged) {
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(OpenGLSandbox::Vertex), &vertexData.front());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexData.size() * sizeof(int), &indexData.front());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		hasVertexDataChanged = false;

		std::cout << "Submitting vertex data" << std::endl;
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
	glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}