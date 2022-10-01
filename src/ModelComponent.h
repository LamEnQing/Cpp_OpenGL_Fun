#pragma once
#include "Component.h"

namespace OpenGLFun {
	class ModelComponent : public IComponent {
	public:
		enum class Type {
			Cube, Axis, TwoD, ThreeD, Maximum
		};

		Type mModelType;
		std::string mModelFilepath;
		bool mEnableBlend;
		bool mShouldCull;

		ModelComponent() : IComponent(), mModelType{ Type::Cube }, mModelFilepath(""), mEnableBlend{false}, mShouldCull{false}  {
			mCompType = ComponentType::Model;
		}
		ModelComponent(EntityId const& owner, Type modelType) : ModelComponent() {
			mOwner = owner;

			mModelType = modelType;
		}
		~ModelComponent() override {}
		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (!jsonObj.HasMember("model") || !jsonObj["model"].IsInt())
				throw SimpleException("Component of type Model must have key 'model' with an integer value");

			int typeMax = static_cast<int>(Type::Maximum);
			int modelVal = jsonObj["model"].GetInt();
			if (modelVal >= typeMax)
				throw SimpleException("Component of type Model's 'model' exceeded the maximum possible value " + std::to_string(typeMax - 1));

			mModelType = static_cast<Type>(modelVal);

			if (jsonObj.HasMember("enable_blend")) {
				if (!jsonObj["enable_blend"].IsBool())
					throw SimpleException("Component of type Model's 'enable_blend' must be a boolean");
				mEnableBlend = jsonObj["enable_blend"].GetBool();
			}

			if (jsonObj.HasMember("should_cull")) {
				if (!jsonObj["should_cull"].IsBool())
					throw SimpleException("Component of type Model's 'should_cull' must be a boolean");
				mShouldCull = jsonObj["should_cull"].GetBool();
			}

			if (jsonObj.HasMember("model_filename")) {
				if (!jsonObj["model_filename"].IsString())
					throw SimpleException("Component of type Model's 'model_filename' must be a string");
				mModelFilepath = jsonObj["model_filename"].GetString();
			}
		}
	};
}