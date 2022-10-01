#pragma once
#include "pch.h"

#include "Vec2f.h"
#include "Vertex.h"

namespace OpenGLFun {
	struct Model {
		Model& Init(std::vector<Vertex>& vertices);
		Model& Destroy();

		Model& Draw2D(unsigned int& shaderProgram, glm::mat4& transformMtx, unsigned int textureId, Vec2f uvDimensions = { 1.0f, 1.0f }, Vec2f uvOffsetPos = { 0.0f, 0.0f }, Vec4f tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		Model& Draw3D(unsigned int& shaderProgram, glm::mat4& modelMtx, glm::mat4& viewMtx, glm::mat4& projMtx, unsigned int textureId, Vec4f tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		Model& SetDrawMode(const int& mode);
		Model& SetCull(const bool& cull);
		Model& SetBlend(const bool& blend);

		private:
			unsigned int _vbo{ 0 }, _vao{ 0 };
			int _vertexCount{ 0 };

			int _drawMode{ GL_TRIANGLES };
			bool _shouldCull{ true };
			bool _shouldBlend{ false };
	};
};