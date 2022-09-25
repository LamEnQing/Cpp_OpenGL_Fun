#pragma once
#include "pch.h"
#include "Component.h"

namespace OpenGLFun {
	class Transform : public IComponent {
	public:
		glm::vec3 mPositionOld;
		glm::vec3 mPosition;
		glm::vec3 mRotationOld;
		glm::vec3 mRotation;
		glm::vec3 mScale;

		Transform() : IComponent(), mPositionOld{}, mPosition{}, mRotationOld{}, mRotation{}, mScale{1.0f} {
			mCompType = ComponentType::Transform;
			std::cout << "Transform constructor\n";
		}
		Transform(EntityId const& owner, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : Transform() {
			mOwner = owner;

			mPositionOld = mPosition = pos;
			mRotationOld = mRotation = rot;
			mScale = scale;
		}
		~Transform() override {}
		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (jsonObj.HasMember("position")) {
				if (!jsonObj["position"].IsArray() || jsonObj["position"].Size() < 3)
					throw SimpleException("Component of type Transform must have key 'position' with an array of size 3");

				const rapidjson::Value& posArr = jsonObj["position"];
				for (int i = 0; i < 3; i++) {
					if (!posArr[i].IsDouble())
						throw JsonReadException("component of type Transform", std::string("size[") + std::to_string(i) + "]", "double");

					mPosition[i] = static_cast<float>(posArr[i].GetDouble());
				}
			}

			if (jsonObj.HasMember("rotation")) {
				if (!jsonObj["rotation"].IsArray() || jsonObj["rotation"].Size() < 3)
					throw SimpleException("Component of type Transform must have key 'rotation' with an array of size 3");

				const rapidjson::Value& rotArr = jsonObj["rotation"];
				for (int i = 0; i < 3; i++) {
					if (!rotArr[i].IsDouble())
						throw JsonReadException("component of type Transform", std::string("size[") + std::to_string(i) + "]", "double");

					mRotation[i] = static_cast<float>(rotArr[i].GetDouble());
				}
			}

			if (jsonObj.HasMember("scale")) {
				if (!jsonObj["scale"].IsArray() || jsonObj["scale"].Size() < 3)
					throw SimpleException("Component of type Transform must have key 'scale' with an array of size 3");

				const rapidjson::Value& scaleArr = jsonObj["scale"];
				for (int i = 0; i < 3; i++) {
					if (!scaleArr[i].IsDouble())
						throw JsonReadException("component of type Transform", std::string("size[") + std::to_string(i) + "]", "double");

					mScale[i] = static_cast<float>(scaleArr[i].GetDouble());
				}
			}
		}
	};
}