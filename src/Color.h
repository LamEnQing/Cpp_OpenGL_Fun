#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec4f.h"

namespace OpenGLFun {
	class Color : public IComponent {
	public:
		Vec4f mRgba;

		Color() : IComponent(), mRgba{ 1.0f } {
			mCompType = ComponentType::Color;
			std::cout << "Color constructor\n";
		}

		~Color() override {}

		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (!jsonObj.HasMember("color") || !jsonObj["color"].IsArray() || jsonObj["color"].Size() < 3)
				throw SimpleException("Component of type Color must have key 'color' with an array of size 3");

			const rapidjson::Value& colorArr = jsonObj["color"];
			for (int i = 0; i < 4; i++) {
				if (colorArr.Size() == 3 && i == 3) continue; // skip alpha if size is 3 and i is 4

				float value;
				if (!colorArr[i].IsFloat() && !colorArr[i].IsInt())
					throw JsonReadException("component of type Color", std::string("color[") + std::to_string(i) + "]", "integer or float");

				if (colorArr[i].IsInt())
					value = static_cast<float>(colorArr[i].GetInt()) / 255.0f;
				else
					value = colorArr[i].GetFloat();

				mRgba[i] = value; // vec4 is double, even though in json, the values are integer
			}
		}
	};
}