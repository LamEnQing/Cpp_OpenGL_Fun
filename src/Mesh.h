#pragma once
#include "pch.h"
#include "Shape.h"

namespace OpenGLFun {
	class Mesh {
	public:
		Mesh();

		Mesh& Init(std::vector<std::shared_ptr<IShape>>& shapes);

		Mesh& Init(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		Mesh& Destroy();

		Mesh& Draw2D(unsigned int& shaderProgram, glm::mat4& transformMtx, unsigned int textureId, Vec2f uvDimensions, Vec2f uvOffsetPos, Vec4f tintColor);
		Mesh& Draw3D(unsigned int& shaderProgram, glm::mat4& modelMtx, glm::mat4& viewMtx, glm::mat4& projMtx, unsigned int textureId, Vec4f tintColor);

		Mesh& SetDrawMode(const int& mode);
		Mesh& SetCull(const bool& cull);
		Mesh& SetBlend(const bool& blend);

		void DeserializeJson(rapidjson::Value const& jsonObj);

		void DeserializeObj(std::string const& filepath);
	private:
		unsigned int _vbo{ 0 }, _vao{ 0 }, _ebo{ 0 };
		int _vertexCount{ 0 }, _indexCount{ 0 };

		int _drawMode{ GL_TRIANGLES };
		bool _shouldCull{ true };
		bool _shouldBlend{ false };
	};
}