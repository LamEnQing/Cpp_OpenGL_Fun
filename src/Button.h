#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec4f.h"

namespace OpenGLFun {
	class Button : public IComponent {
	public:
		Vec4f mHoverRgba;
		std::array<int, 2> mHoverUVPos;
		std::array<int, 2> mHoverUVDimensions;
		Button() : IComponent(), mHoverRgba{ 1.0f, 1.0f, 1.0f, 1.0f }, mHoverUVPos{ 0, 0 }, mHoverUVDimensions{ 0, 0 } { mCompType = ComponentType::Button; }

		~Button() override {}
		void Deserialize(rapidjson::Value const& jsonObj) {
			if (!jsonObj.HasMember("hover_color") || !jsonObj["hover_color"].IsArray() || jsonObj["hover_color"].Size() < 3)
				throw SimpleException("Component of type Button must have key 'hover_color' with an array of size 3");

			const rapidjson::Value& colorArr = jsonObj["hover_color"];
			for (int i = 0; i < 4; i++) {
				if (colorArr.Size() == 3 && i == 3) continue; // skip alpha if size is 3 and i is 4

				float value;
				if (!colorArr[i].IsFloat() && !colorArr[i].IsInt())
					throw JsonReadException("component of type Button", std::string("hover_color[") + std::to_string(i) + "]", "integer or float");

				if (colorArr[i].IsInt())
					value = static_cast<float>(colorArr[i].GetInt()) / 255.0f;
				else
					value = colorArr[i].GetFloat();

				mHoverRgba[i] = value; // vec4 is double, even though in json, the values are integer
			}
		}
	};
}