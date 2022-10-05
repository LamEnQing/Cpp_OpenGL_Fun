#pragma once
#include "Component.h"

namespace OpenGLFun {
	class ModelComponent : public IComponent {
	public:
		std::string mModelFilepath;
		bool mEnableBlend;
		bool mShouldCull;

		ModelComponent() : IComponent(), mModelFilepath(""), mEnableBlend{false}, mShouldCull{false}  {
			mCompType = ComponentType::Model;
		}
		ModelComponent(EntityId const& owner) : ModelComponent() {
			mOwner = owner;
		}
		~ModelComponent() override {}
		void Deserialize(rapidjson::Value const& jsonObj) override {
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

			if (jsonObj.HasMember("model")) {
				if (!jsonObj["model"].IsString())
					throw SimpleException("Component of type Model's 'model' must be a string");
				mModelFilepath = jsonObj["model"].GetString();
			}
		}
	};
}