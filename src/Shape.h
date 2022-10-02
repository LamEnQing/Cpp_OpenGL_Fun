#pragma once
#include "pch.h"
#include "Vertex.h"
#include "Vec4f.h"

namespace OpenGLFun {

	class Shape {
	public:
		using ElementIndex = unsigned int;

		Shape();
		Shape(std::vector<Vertex> vertices, std::vector<ElementIndex> indices, Vec4f color);

		const std::vector<Vertex>& Vertices() const;
		std::vector<ElementIndex>& Indices();

		Shape& Deserialize(rapidjson::Value const& jsonObj);
	private:
		std::vector<Vertex> _vertices;
		std::vector<ElementIndex> _indices;
		Vec4f _color;
	};
}