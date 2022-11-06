#include "pch.h"
#include "Model.h"

#include "BasicShape.h"
#include "Serializer.h"
#include "WindowSystem.h"

namespace OpenGLFun {
	Model::Model() : _meshes{} {}

	Model& Model::AddMesh(std::string meshName, std::shared_ptr<Mesh> mesh) {
		_meshes.insert({ meshName, mesh });

		return *this;
	}

	Mesh* Model::GetMesh(std::string meshName) {
		if (_meshes.find(meshName) == _meshes.end())
			return nullptr;

		return _meshes.at(meshName).get();
	}

	std::map<std::string, std::shared_ptr<Mesh>>& Model::GetMeshMap() {
		return _meshes;
	}

	Model& Model::Destroy() {
		_meshes.clear();

		return *this;
	}

	Model& Model::Draw2D(unsigned int& shaderProgram, glm::mat4 const& transformMtx, unsigned int textureId, Vec2f uvDimensions, Vec2f uvOffsetPos, Vec4f tintColor) {
		for (auto& meshPair : _meshes) {
			meshPair.second->Draw2D(shaderProgram, transformMtx, textureId, uvDimensions, uvOffsetPos, tintColor);
		}

		return *this;
	}

	Model& Model::Draw3D(unsigned int& shaderProgram, glm::mat4 const& modelMtx, glm::mat4 const& viewMtx, glm::mat4 const& projMtx, unsigned int textureId, Vec4f tintColor) {
		for (auto& meshPair : _meshes) {
			meshPair.second->Draw3D(shaderProgram, modelMtx, viewMtx, projMtx, textureId, tintColor);
		}

		return *this;
	}

	Model& Model::SetCull(const bool& cull) {
		for (auto& meshPair : _meshes) {
			meshPair.second->SetCull(cull);
		}

		return *this;
	}

	Model& Model::SetBlend(const bool& blend) {
		for (auto& meshPair : _meshes) {
			meshPair.second->SetBlend(blend);
		}

		return *this;
	}
}