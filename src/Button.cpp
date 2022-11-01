#include "Button.h"

namespace OpenGLFun {
	Button::Button() : IComponent(), mHoverRgba{ 1.0f, 1.0f, 1.0f, 1.0f }, mHoverUVPos{ 0, 0 }, mHoverUVDimensions{ 0, 0 } { mCompType = ComponentType::Button; }

	Button::~Button() {}
	void Button::Deserialize(rapidjson::Value const& jsonObj) {
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

		if (jsonObj.HasMember("hover_uv_pos")) {
			if (!jsonObj["hover_uv_pos"].IsArray() || jsonObj["hover_uv_pos"].Size() < 2)
				throw SimpleException("Component of type Button must have key 'hover_uv_pos' with an array of size 2");

			const rapidjson::Value& posArr = jsonObj["hover_uv_pos"];
			for (int i = 0; i < 2; i++) {
				if (!posArr[i].IsInt())
					throw JsonReadException("component of type Button", std::string("hover_uv_pos[") + std::to_string(i) + "]", "integer");

				mHoverUVPos[i] = posArr[i].GetInt();
			}
		}

		if (jsonObj.HasMember("hover_uv_dimensions")) {
			if (!jsonObj["hover_uv_dimensions"].IsArray() || jsonObj["hover_uv_dimensions"].Size() < 2)
				throw SimpleException("Component of type Button must have key 'hover_uv_dimensions' with an array of size 2");

			const rapidjson::Value& dimArr = jsonObj["hover_uv_dimensions"];
			for (int i = 0; i < 2; i++) {
				if (!dimArr[i].IsInt())
					throw JsonReadException("component of type Button", std::string("hover_uv_dimensions[") + std::to_string(i) + "]", "integer");

				mHoverUVDimensions[i] = dimArr[i].GetInt();
			}
		}
	}

	bool Button::DrawImGuiComponent() {
		bool canClose = true;
		if (!ImGui::CollapsingHeader("Button", &canClose)) return !canClose;
		ImGui::Text("Hover Color");
		ImGui::SameLine();
		ImGui::Text("Red");
		ImGui::Text("Green");
		ImGui::Text("Blue");
		ImGui::Text("Alpha");

		return !canClose;
	}
}