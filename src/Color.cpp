#include "Color.h"

namespace OpenGLFun {
	Color::Color() : IComponent(), mRgba{ 1.0f } {
		mCompType = ComponentType::Color;
	}

	Color::~Color() {}

	void Color::Deserialize(rapidjson::Value const& jsonObj) {
		if (!jsonObj.HasMember("color") || !jsonObj["color"].IsArray() || jsonObj["color"].Size() < 3)
			throw JsonReadException("component of type Color", "color", "array with minimum 3 elements");
		if (jsonObj["color"].Size() > 4)
			throw JsonReadException("component of type Color", "color", "array with maximum 4 elements");

		const rapidjson::Value& colorArr = jsonObj["color"].GetArray();
		for (rapidjson::SizeType i = 0; i < colorArr.Size(); i++) {
			float value;
			if (!colorArr[i].IsNumber())
				throw JsonReadException("component of type Color", std::string("color[") + std::to_string(i) + "]", "integer or float");

			if (colorArr[i].IsInt())
				value = static_cast<float>(colorArr[i].GetInt()) / 255.0f;
			else
				value = colorArr[i].GetFloat();

			mRgba[i] = value; // vec4 is double, even though in json, the values are integer
		}
	}

	bool Color::DrawImGuiComponent() {
		bool canClose = true;
		if (!ImGui::CollapsingHeader("Color", &canClose)) return !canClose;
		return !canClose;
	}
}