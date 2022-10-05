#include "CuboidShape.h"

namespace OpenGLFun {
	static std::array<std::string, 6> FACE_NAMES = { "bottom_face", "top_face", "positive_z_face", "negative_z_face", "negative_x_face", "positive_x_face" };

	CuboidShape::CuboidShape() {}
	CuboidShape::~CuboidShape() {}

	void CuboidShape::Deserialize(rapidjson::Value const& jsonObj) {
		Vec3f localOrigin(0.0f), offset(0.0f), size(1.0f);
		std::array<Vec4f, 6> colors = {
			Vec4f(1.0f), Vec4f(1.0f), Vec4f(1.0f),
			Vec4f(1.0f), Vec4f(1.0f), Vec4f(1.0f)
		};

		if (jsonObj.HasMember("local_origin")) {
			if (!jsonObj["local_origin"].IsArray() || jsonObj["local_origin"].Size() != 3)
				throw JsonReadException("CuboidShape", "local_origin", "array with 3 elements");

			const rapidjson::Value& originArr = jsonObj["local_origin"].GetArray();
			for (rapidjson::SizeType i = 0; i < originArr.Size(); i++) {
				if (!originArr[i].IsNumber())
					throw JsonReadException("CuboidShape", "local_origin[" + std::to_string(i) + "]", "number");

				localOrigin[i] = originArr[i].GetFloat();
			}
		}

		if (jsonObj.HasMember("offset")) {
			if (!jsonObj["offset"].IsArray() || jsonObj["offset"].Size() != 3)
				throw JsonReadException("CuboidShape", "offset", "array with 3 elements");

			const rapidjson::Value& offsetArr = jsonObj["offset"].GetArray();
			for (rapidjson::SizeType i = 0; i < offsetArr.Size(); i++) {
				if (!offsetArr[i].IsNumber())
					throw JsonReadException("CuboidShape", "offset[" + std::to_string(i) + "]", "number");

				offset[i] = offsetArr[i].GetFloat();
			}
		}

		if (jsonObj.HasMember("colors")) {
			if (!jsonObj["colors"].IsObject())
				throw JsonReadException("CuboidShape", "colors", "JSON object");

			const rapidjson::Value& colorsObj = jsonObj["colors"];

			for (size_t i = 0; i < FACE_NAMES.size(); i++) {
				const char* faceName = FACE_NAMES.at(i).c_str();
				if (!colorsObj.HasMember(faceName)) continue;
				if (!colorsObj[faceName].IsArray() || colorsObj[faceName].Size() != 3)
					throw JsonReadException("CuboidShape", "colors", faceName, "array with 3 elements");

				const rapidjson::Value& colorArr = colorsObj[faceName].GetArray();
				for (rapidjson::SizeType j = 0; j < colorArr.Size(); j++) {
					if (!colorArr[j].IsNumber())
						throw JsonReadException("CuboidShape", "colors", std::string(faceName) + "[" + std::to_string(j) + "]", "number");

					float value;
					if (colorArr[j].IsInt()) {
						value = colorArr[j].GetFloat() / 255.0f;
					}
					else {
						value = colorArr[j].GetFloat();
					}
					colors[i][j] = value;
				}
			}
		}

		if (!jsonObj.HasMember("size") || !jsonObj["size"].IsArray() || jsonObj["size"].Size() != 3)
			throw JsonReadException("CuboidShape", "size", "array with 3 elements");

		const rapidjson::Value& sizeArr = jsonObj["size"].GetArray();
		for (rapidjson::SizeType i = 0; i < sizeArr.Size(); i++) {
			if (!sizeArr[i].IsNumber())
				throw JsonReadException("CuboidShape", "size[" + std::to_string(i) + "]", "number");

			size[i] = sizeArr[i].GetFloat();
		}

		Vertex vertex;
		_vertices = {
			// bottom face (-ve y), blue
			vertex.Pos(0.0f, 0.0f, 0.0f).Color(0.0f, 0.0f, 1.0f),
			vertex.Pos(size.x, 0.0f, 0.0f),
			vertex.Pos(size.x, 0.0f, size.z),
			vertex.Pos(0.0f, 0.0f, size.z),

			// top face (-ve y), cyan
			vertex.Pos(size.x, size.y, 0.0f).Color(0.0f, 1.0f, 1.0f),
			vertex.Pos(0.0f, size.y, 0.0f),
			vertex.Pos(0.0f, size.y, size.z),
			vertex.Pos(size.x, size.y, size.z),

			// front face (+ve z), green
			vertex.Pos(0.0,  size.y, size.z).Color(0.0f, 1.0f, 0.0f),
			vertex.Pos(0.0f, 0.0f, size.z),
			vertex.Pos(size.x, 0.0f, size.z),
			vertex.Pos(size.x, size.y, size.z),

			// back face (-ve z), yellow
			vertex.Pos(size.x, size.y, 0.0f).Color(1.0f, 1.0f, 0.0f),
			vertex.Pos(size.x, 0.0f, 0.0f),
			vertex.Pos(0.0f, 0.0f, 0.0f),
			vertex.Pos(0.0f, size.y, 0.0f),

			// left face (-ve x), purple
			vertex.Pos(0.0f,  size.y, 0.0f).Color(1.0f, 0.0f, 1.0f),  // top left
			vertex.Pos(0.0f, 0.0f, 0.0f),  // bottom left
			vertex.Pos(0.0f, 0.0f, size.z),  // bottom left
			vertex.Pos(0.0f, size.y, size.z),  // bottom right

			// right face (+ve x), red
			vertex.Pos(size.x, size.y, size.z).Color(1.0f, 0.0f, 0.0f),
			vertex.Pos(size.x, 0.0f, size.z),
			vertex.Pos(size.x, 0.0f, 0.0f),
			vertex.Pos(size.x, size.y, 0.0f)
		};

		for (ElementIndex i = 0; i < _vertices.size() / 4; i++) {
			ElementIndex start = i * 4;
			_indices.insert(_indices.end(), {
				start + 0, start + 1, start + 2,
				start + 2, start + 3, start + 0
			});
			for (int j = 0; j < 4; j++) {
				_vertices[start + j].mColor = colors[i];
			}
		}

		for (Vertex& vert : _vertices) {
			vert.mPos += localOrigin + offset;
		}
	}
}