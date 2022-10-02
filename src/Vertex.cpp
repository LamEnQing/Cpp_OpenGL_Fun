#include "Vertex.h"

namespace OpenGLFun {
	Vertex& Vertex::Pos(float x, float y, float z) {
		this->mPos = { x, y, z };

		return *this;
	}

	Vertex& Vertex::Color(float red, float green, float blue, float alpha) {
		this->mColor = { red, green, blue, alpha };

		return *this;
	}

	Vertex& Vertex::UV(float u, float v) {
		this->mUV = { u, v };

		return *this;
	}

	void Vertex::Deserialise(rapidjson::Value const& jsonObj, bool omitColor) {
		if (!jsonObj.HasMember("position") || !jsonObj["position"].IsArray() || jsonObj["position"].Size() != 3)
			throw JsonReadException("Vertex", "position", "array with 3 elements");

		const auto& posArr = jsonObj["position"].GetArray();
		for (int i = 0; i < posArr.Size(); i++) {
			if (!posArr[i].IsFloat())
				throw JsonReadException("Vertex", std::string("position[") + std::to_string(i) + "]", "float");

			mPos[i] = posArr[i].GetFloat();
		}

		if (!omitColor) {
			if (!jsonObj.HasMember("color") || !jsonObj["color"].IsArray() || jsonObj["color"].Size() < 3)
				throw JsonReadException("Vertex", "color", "array with minimum 3 elements");
			if (jsonObj["color"].Size() > 4)
				throw JsonReadException("Vertex", "color", "array with maximum 4 elements");

			const auto& colorArr = jsonObj["color"].GetArray();
			for (rapidjson::SizeType i = 0; i < colorArr.Size(); i++) {
				float value;
				if (!colorArr[i].IsNumber())
					throw JsonReadException("Vertex", std::string("color[") + std::to_string(i) + "]", "integer or float");

				if (colorArr[i].IsInt())
					value = static_cast<float>(colorArr[i].GetInt()) / 255.0f;
				else
					value = colorArr[i].GetFloat();

				mColor[i] = value;
			}
		}

		if (jsonObj.HasMember("uv")) {
			if (!jsonObj["uv"].IsArray() || jsonObj["uv"].Size() != 2)
				throw JsonReadException("Vertex", "uv", "array with 2 elements");

			const auto& uvArr = jsonObj["uv"].GetArray();
			for (int i = 0; i < uvArr.Size(); i++) {
				if (!uvArr[i].IsFloat())
					throw JsonReadException("Vertex", std::string("uv[") + std::to_string(i) + "]", "float");

				mUV[i] = uvArr[i].GetFloat();
			}
		}
	}
}