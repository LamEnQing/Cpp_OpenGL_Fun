// Most code from: https://www.youtube.com/watch?v=Th4huqR77rI
#pragma once

#include <glad/glad.h>

#include <numeric>
#include <vector>

#include "ShaderProgram.h"
#include "Vertex.h"

class BatchRendering {
private:
	unsigned int uint_max = std::numeric_limits<unsigned int>::max();

	// Vertex Array Object, Vertex Buffer Object, Index Buffer Object
	unsigned int vao{ uint_max }, vbo{ uint_max }, ibo{ uint_max };

	GLuint sinonSqTex{ UINT32_MAX }, gwenStacyTex{ UINT32_MAX };

	bool hasVertexDataChanged{ false };
	std::vector<OpenGLSandbox::Vertex> vertexData;
	std::vector<uint32_t> indexData;

	const size_t maxQuad{ 100 };
	size_t quadCount{ 0 };
public:
	BatchRendering();
	~BatchRendering();

	void Draw(OpenGLSandbox::ShaderProgram& shdrPgm);
};