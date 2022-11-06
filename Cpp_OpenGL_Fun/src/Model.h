#pragma once
#include "pch.h"

#include "Mesh.h"

namespace OpenGLFun {
	struct Model {
		Model();

		Model& AddMesh(std::string meshName, std::shared_ptr<Mesh> mesh);
		Mesh* GetMesh(std::string meshName);
		std::map<std::string, std::shared_ptr<Mesh>>& GetMeshMap();

		Model& Destroy();

		Model& Draw2D(unsigned int& shaderProgram, glm::mat4 const& transformMtx, unsigned int textureId, Vec2f uvDimensions, Vec2f uvOffsetPos, Vec4f tintColor);
		Model& Draw3D(unsigned int& shaderProgram, glm::mat4 const& modelMtx, glm::mat4 const& viewMtx, glm::mat4 const& projMtx, unsigned int textureId, Vec4f tintColor);

		Model& SetCull(const bool& cull);
		Model& SetBlend(const bool& blend);

		private:
			std::map<std::string, std::shared_ptr<Mesh>> _meshes;
	};
};