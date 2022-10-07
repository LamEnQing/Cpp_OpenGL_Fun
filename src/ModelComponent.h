#pragma once
#include "Component.h"

namespace OpenGLFun {
	// decides which shader and draw function to use
	enum class ModelType {
		TwoD, ThreeD
	};

	class ModelComponent : public IComponent {
	public:
		ModelType mModelType;
		std::string mModelFilepath;
		bool mEnableBlend;
		bool mShouldCull;

		ModelComponent() : IComponent(), mModelType(ModelType::TwoD), mModelFilepath(""), mEnableBlend{false}, mShouldCull{false}  {
			mCompType = ComponentType::Model;
		}
		ModelComponent(EntityId const& owner) : ModelComponent() {
			mOwner = owner;
		}
		~ModelComponent() override {}
		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (!jsonObj.HasMember("model_type") || !jsonObj["model_type"].IsString())
				throw SimpleException("Component of type Model's 'model_type' must be a string");

			std::string modelType = jsonObj["model_type"].GetString();
			for (char& c : modelType) {
				c = static_cast<char>(std::tolower(c));
			}
			if (!(modelType == "2d" || modelType == "3d"))
				throw SimpleException("In component of type Model's 'model_type', possible values are \"2d\" or \"3d\"");
			mModelType = modelType == "2d" ? ModelType::TwoD : ModelType::ThreeD;

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

			if (!jsonObj.HasMember("model") || !jsonObj["model"].IsString())
				throw SimpleException("Component of type Model's 'model' must be a string");
			mModelFilepath = jsonObj["model"].GetString();
		}
	};
}