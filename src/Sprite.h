#pragma once
#include "pch.h"
#include "Component.h"

namespace OpenGLFun {
	class Sprite : public IComponent {
	public:
		std::string mTextureFilepath;
		std::array<int, 2> mUVPosition;
		std::array<int, 2> mUVDimensions;

		Sprite() : IComponent(), mTextureFilepath(""), mUVPosition{ 0, 0 }, mUVDimensions{ 0, 0 } {
			mCompType = ComponentType::Sprite;
			std::cout << "Sprite constructor\n";
		}
		~Sprite() override {}

		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (!jsonObj.HasMember("texture") || !jsonObj["texture"].IsString())
				throw SimpleException("Component of type Sprite must have key 'texture' with a string value");

			mTextureFilepath = jsonObj["texture"].GetString();

			if (jsonObj.HasMember("uv_pos")) {
				if (!jsonObj["uv_pos"].IsArray() || jsonObj["uv_pos"].Size() < 2)
					throw SimpleException("Component of type Sprite must have key 'uv_pos' with an array of size 2");

				const rapidjson::Value& posArr = jsonObj["uv_pos"];
				for (int i = 0; i < 2; i++) {
					if (!posArr[i].IsInt())
						throw JsonReadException("component of type Sprite", std::string("uv_pos[") + std::to_string(i) + "]", "integer");

					mUVPosition[i] = posArr[i].GetInt();
				}
			}

			if (jsonObj.HasMember("uv_dimensions")) {
				if (!jsonObj["uv_dimensions"].IsArray() || jsonObj["uv_dimensions"].Size() < 2)
					throw SimpleException("Component of type Sprite must have key 'uv_dimensions' with an array of size 2");

				const rapidjson::Value& dimArr = jsonObj["uv_dimensions"];
				for (int i = 0; i < 2; i++) {
					if (!dimArr[i].IsInt())
						throw JsonReadException("component of type Sprite", std::string("uv_dimensions[") + std::to_string(i) + "]", "integer");

					mUVDimensions[i] = dimArr[i].GetInt();
				}
			}
		}
	};
}
