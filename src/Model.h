#pragma once
#include "pch.h"

#include "Mesh.h"

namespace OpenGLFun {
	struct Model {
		Model();

		Model& AddMesh(std::string meshName, std::shared_ptr<Mesh> mesh);
		Model& Destroy();

		Model& Draw2D(unsigned int& shaderProgram, glm::mat4& transformMtx, unsigned int textureId, Vec2f uvDimensions, Vec2f uvOffsetPos, Vec4f tintColor);
		Model& Draw3D(unsigned int& shaderProgram, glm::mat4& modelMtx, glm::mat4& viewMtx, glm::mat4& projMtx, unsigned int textureId, Vec4f tintColor);

		Model& SetCull(const bool& cull);
		Model& SetBlend(const bool& blend);

		private:
			std::map<std::string, std::shared_ptr<Mesh>> _meshes;
	};
};