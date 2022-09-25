#pragma once
#include "Component.h"

namespace OpenGLFun {
	class ModelComponent : public IComponent {
	public:
		enum class Type {
			Cube, Axis, TwoD, Maximum
		};

		Type mModelType;
		bool mEnableBlend;

		ModelComponent() : IComponent(), mModelType{ Type::Cube }, mEnableBlend{ false } {
			mCompType = ComponentType::Model;
			std::cout << "Model constructor\n";
		}
		ModelComponent(EntityId const& owner, Type modelType) : ModelComponent() {
			mOwner = owner;

			mModelType = modelType;
		}
		~ModelComponent() override {}
		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (!jsonObj.HasMember("model") || !jsonObj["model"].IsInt())
				throw SimpleException("Component of type Model must have key 'model' with an integer");

			int typeMax = static_cast<int>(Type::Maximum);
			int modelVal = jsonObj["model"].GetInt();
			if (modelVal >= typeMax)
				throw SimpleException("Component of type Model's 'model' exceeded the maximum possible value of " + std::to_string(typeMax - 1));

			mModelType = static_cast<Type>(modelVal);

			if (jsonObj.HasMember("enable_blend")) {
				if (!jsonObj["enable_blend"].IsBool())
					throw SimpleException("Component of type Model's 'enable_blend' must be a boolean");
				mEnableBlend = jsonObj["enable_blend"].GetBool();
			}
		}
	};
}