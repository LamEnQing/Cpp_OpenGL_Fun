#pragma once
#include "pch.h"
#include "Shape.h"

namespace OpenGLFun {
	class Mesh {
	public:
		Mesh();
		~Mesh();

		Mesh* Init(std::vector<std::shared_ptr<IShape>>& shapes);

		Mesh* Init(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

		Mesh* Draw2D(unsigned int& shaderProgram, glm::mat4 const& transformMtx, unsigned int textureId, Vec2f uvDimensions, Vec2f uvOffsetPos, Vec4f tintColor);
		Mesh* Draw3D(unsigned int& shaderProgram, glm::mat4 const& modelMtx, glm::mat4 const& viewMtx, glm::mat4 const& projMtx, unsigned int textureId, Vec4f tintColor);

		Mesh* SetDrawMode(const int& mode);
		Mesh* SetCull(const bool& cull);
		Mesh* SetBlend(const bool& blend);
		Mesh* SetOffset(Vec3f& vec);

		Mesh* SetRotation(Vec3f& vec);
		Vec3f const& GetRotation() const;

		void DeserializeJson(rapidjson::Value const& jsonObj);

		void DeserializeObj(std::string const& filepath);
	private:
		unsigned int _vbo{ UINT32_MAX }, _vao{ UINT32_MAX }, _ebo{ UINT32_MAX };
		int _vertexCount{ 0 }, _indexCount{ 0 };

		int _drawMode{ GL_TRIANGLES };
		bool _shouldCull{ true };
		bool _shouldBlend{ false };

		Vec3f _offset; // move the origin of this mesh
		Vec3f _rotation; // rotate the mesh
	};
}