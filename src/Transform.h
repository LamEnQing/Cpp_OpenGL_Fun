#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec3f.h"

namespace OpenGLFun {
	class Transform : public IComponent {
	public:
		Vec3f mPositionOld;
		Vec3f mPosition;
		Vec3f mRotationOld;
		Vec3f mRotation;
		Vec3f mScale;

		Transform() : IComponent(), mPositionOld{}, mPosition{}, mRotationOld{}, mRotation{}, mScale{ 1.0f } {
			mCompType = ComponentType::Transform;
		}
		Transform(EntityId const& owner, Vec3f pos, Vec3f rot, Vec3f scale) : Transform() {
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
					if (!posArr[i].IsNumber())
						throw JsonReadException("component of type Transform", std::string("position[") + std::to_string(i) + "]", "float");

					mPosition[i] = posArr[i].GetFloat();
				}
			}

			if (jsonObj.HasMember("rotation")) {
				if (!jsonObj["rotation"].IsArray() || jsonObj["rotation"].Size() < 3)
					throw SimpleException("Component of type Transform must have key 'rotation' with an array of size 3");

				const rapidjson::Value& rotArr = jsonObj["rotation"];
				for (int i = 0; i < 3; i++) {
					if (!rotArr[i].IsNumber())
						throw JsonReadException("component of type Transform", std::string("rotation[") + std::to_string(i) + "]", "float");

					mRotation[i] = rotArr[i].GetFloat();
				}
			}

			if (jsonObj.HasMember("scale")) {
				if (!jsonObj["scale"].IsArray() || jsonObj["scale"].Size() < 3)
					throw SimpleException("Component of type Transform must have key 'scale' with an array of size 3");

				const rapidjson::Value& scaleArr = jsonObj["scale"];
				for (int i = 0; i < 3; i++) {
					if (!scaleArr[i].IsNumber())
						throw JsonReadException("component of type Transform", std::string("scale[") + std::to_string(i) + "]", "float");

					mScale[i] = scaleArr[i].GetFloat();
				}
			}
		}

		void DrawImGuiComponent() {

		}
	};
}