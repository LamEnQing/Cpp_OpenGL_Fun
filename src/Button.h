#pragma once
#include "pch.h"
#include "Component.h"

namespace OpenGLFun {
	class Button : public IComponent {
	public:
		glm::vec4 mHoverRgba;

		Button() : IComponent(), mHoverRgba{ 1 } { mCompType = ComponentType::Button; }
		Button(EntityId& owner, glm::vec4 hoverRgba) : Button() {
			mOwner = owner;
			mHoverRgba = hoverRgba;
		}
		~Button() override {}
		void Deserialize(rapidjson::Value const& jsonObj) {
			if (!jsonObj.HasMember("hover_color") || !jsonObj["hover_color"].IsArray() || jsonObj["hover_color"].Size() < 3)
				throw SimpleException("Component of type Button must have key 'hover_color' with an array of size 3");

			const rapidjson::Value& colorArr = jsonObj["hover_color"];
			for (int i = 0; i < 4; i++) {
				if (colorArr.Size() == 3 && i == 3) continue; // skip alpha if size is 3 and i is 4

				float value;
				if (!colorArr[i].IsDouble() && !colorArr[i].IsInt())
					throw JsonReadException("component of type Button", std::string("hover_color[") + std::to_string(i) + "]", "integer or double");

				if (colorArr[i].IsInt())
					value = static_cast<float>(colorArr[i].GetInt());
				else
					value = static_cast<float>(colorArr[i].GetDouble());

				mHoverRgba[i] = value; // vec4 is double, even though in json, the values are integer
			}
		}
	};
}