// Most code from: https://www.youtube.com/watch?v=Th4huqR77rI
#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>

#include "GlobalDeclared.h"
#include "Mesh.h"
#include "ShaderProgram.h"

namespace OpenGLSandbox {
	class IBatchRenderer {
	protected:
		// Vertex Array Object, Vertex Buffer Object, Index Buffer Object
		unsigned int vao{ uint_max }, vbo{ uint_max }, ibo{ uint_max };

		std::vector<Mesh> meshData;
		std::vector<VertexIndexType> indexData;

		int bufferUpdates{ 0 }; // number of times the buffer has been updated

		// Quick way to create the VAO, VBO and IBO
		void CreateBuffers(size_t maxVertices, size_t maxIndices) {// Create a vertWrap array (VAO)
			glGenVertexArrays(1, &this->vao);
			glBindVertexArray(this->vao);

			// Create Vertex Buffer (VBO) and populate it with data from m_vertices
			glGenBuffers(1, &this->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferData(GL_ARRAY_BUFFER, maxVertices, nullptr, GL_DYNAMIC_DRAW); // 4 m_vertices per quad

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
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndices, nullptr, GL_DYNAMIC_DRAW); // A quad is made up of 2 triangles. A triangle is made up of 3 m_vertices. So 2 * 3 = 6 m_vertices to draw a single quad

			glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
			glBindVertexArray(0); // unbind VAO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO
		}

		// Update the VAO and IBO buffers when the vector of Mesh is updated
		void UpdateBuffers() {
			std::vector<Vertex> vertices;

			for (auto meshIt = meshData.begin(); meshIt != meshData.end(); meshIt++) {
				vertices.reserve(sizeof(Vertex) * meshIt->m_vertices.size());
				for (auto vertexIt = meshIt->m_vertices.begin(); vertexIt != meshIt->m_vertices.end(); vertexIt++)
					vertices.push_back(vertexIt->vertex);
			}

			glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices.front());
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexData.size() * sizeof(VertexIndexType), &indexData.front());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			bufferUpdates++;

			std::cout << "Updating buffer, update count:" << bufferUpdates << std::endl;
		}
	public:
		IBatchRenderer() {}
		~IBatchRenderer() {
			glDeleteVertexArrays(1, &this->vao);
			glDeleteBuffers(1, &this->vbo);
			glDeleteBuffers(1, &this->ibo);
		}

		virtual void Draw(ShaderProgram& shdrPgm) = 0;
	};
}