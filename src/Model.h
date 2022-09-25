#pragma once
#include "pch.h"

#include "Vertex.h"

namespace OpenGLFun {
	struct Model {
		Model& Init(std::vector<Vertex>& vertices);
		Model& Destroy();

		Model& Draw2D(unsigned int& shaderProgram, glm::mat4& transMtx, glm::vec2& scaleVec, glm::vec4 tintVec=glm::vec4(1.0f));
		Model& Draw3D(unsigned int& shaderProgram, glm::mat4& modelMtx, glm::mat4& viewMtx, glm::mat4& projMtx, glm::vec4 tintVec=glm::vec4(1.0f));

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