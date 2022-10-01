#include "Shape.h"

namespace OpenGLFun {
	Shape::Shape() : _vertices(), _indices{}, _color{} {}
	Shape::Shape(std::vector<Vertex> vertices, std::vector<ElementIndex> indices, Vec4f color) : _vertices(vertices), _indices(indices), _color(color) {
		for (Vertex& vert : _vertices)
			vert.mColor = color;
	}

	const std::vector<Vertex>& Shape::Vertices() const { return _vertices; }
	std::vector<Shape::ElementIndex>& Shape::Indices() { return _indices; }

	Shape Shape::Deserialize(rapidjson::Value const& jsonObj) {
		if (!jsonObj.HasMember("color") || !jsonObj["color"].IsArray() || jsonObj["color"].Size() < 3)
			throw JsonReadException("Shape", "color", "array with minimum 3 elements");
		if (jsonObj["color"].Size() > 4)
			throw JsonReadException("Shape", "color", "array with maximum 4 elements");

		Vec4f color(1.0f);
		const rapidjson::Value& colorArr = jsonObj["color"].GetArray();
		for (rapidjson::SizeType i = 0; i < colorArr.Size(); i++) {
			float value;
			if (!colorArr[i].IsNumber())
				throw JsonReadException("Shape", std::string("color[") + std::to_string(i) + "]", "integer or float");

			if (colorArr[i].IsInt())
				value = static_cast<float>(colorArr[i].GetInt()) / 255.0f;
			else
				value = colorArr[i].GetFloat();

			color[i] = value; // vec4 is float, even though in json, the values are integer
		}

		if (!jsonObj.HasMember("vertices") || !jsonObj["vertices"].IsArray() || jsonObj["vertices"].Size() < 2)
			throw JsonReadException("Shape", "vertices", "array with minimum 2 elements");

		std::vector<Vertex> vertices;
		const rapidjson::Value& vertexArr = jsonObj["vertices"].GetArray();
		for (rapidjson::SizeType i = 0; i < vertexArr.Size(); i++) {
			Vertex vertex;

			if (!vertexArr[i].IsObject())
				throw JsonReadException("Shape", std::string("vertices[") + std::to_string(i) + "]", "JSON object");

			try {
				vertex.Deserialise(vertexArr[i], true);
			}
			catch (std::exception& e) {
				throw SimpleException(std::string("In Shape, here's the parse error for vertices[") + std::to_string(i) + "]: " + e.what());
			}
			vertices.push_back(vertex);
		}

		if (!jsonObj.HasMember("indices") || !jsonObj["indices"].IsArray() || jsonObj["indices"].Size() < 2)
			throw JsonReadException("Shape", "indices", "array with minimum 2 elements");

		std::vector<ElementIndex> indices;
		const rapidjson::Value& indexArr = jsonObj["indices"].GetArray();
		for (rapidjson::SizeType i = 0; i < indexArr.Size(); i++) {
			if (!indexArr[i].IsInt())
				throw JsonReadException("Shape", std::string("indices[") + std::to_string(i) + "]", "integer");

			indices.push_back(static_cast<ElementIndex>(indexArr[i].GetInt()));
		}

		return Shape(vertices, indices, color);
	}
}