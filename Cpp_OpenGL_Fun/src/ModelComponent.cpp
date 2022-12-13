#include "ModelComponent.h"

#include "Engine.h"
#include "StringUtils.h"

namespace OpenGLFun {
	ModelComponent::ModelComponent() : IComponent(), mModelType(ModelType::TwoD), mModelFilepath(""), mEnableBlend{false}, mShouldCull{false}  {
		mCompType = ComponentType::Model;
	}
	ModelComponent::ModelComponent(EntityId const& owner) : ModelComponent() {
		mOwner = owner;
	}
	ModelComponent::~ModelComponent() {}

	// Throws an exception if string is not either '2d' or '3d'. You MUST catch the exception and throw your own error message exception
	ModelType ModelComponent::ParseModelTypeString(std::string typeStr) {
		typeStr = StringUtils::ToLower(typeStr);

		if (!(typeStr == "2d" || typeStr == "3d"))
			throw std::exception();

		return typeStr == "2d" ? ModelType::TwoD : ModelType::ThreeD;
	}

	void ModelComponent::Deserialize(rapidjson::Value const& jsonObj) {
		if (!jsonObj.HasMember("model_type") || !jsonObj["model_type"].IsString())
			throw SimpleException("Component of type Model's 'model_type' must be a string");

		try {
			mModelType = ParseModelTypeString(jsonObj["model_type"].GetString());
		}
		catch (...) {
			throw SimpleException("In component of type Model's 'model_type', possible values are \"2d\" or \"3d\"");
		}

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

	bool ModelComponent::DrawImGuiComponent() {
		bool canClose = true;
		if (!ImGui::CollapsingHeader("Model", &canClose)) return !canClose;
		float offsetFromMainText = 80;

		ImGui::Text("Type"); ImGui::SameLine();
		ImGui::SetCursorPosX(offsetFromMainText);
		if (ImGui::BeginCombo("##ModelType", mModelType == ModelType::TwoD ? "2D" : "3D")) {
			if (ImGui::Selectable("2D", mModelType == ModelType::TwoD))
				mModelType = ModelType::TwoD;
			if (ImGui::Selectable("3D", mModelType == ModelType::ThreeD))
				mModelType = ModelType::ThreeD;
			ImGui::EndCombo();
		}

		ImGui::Text("Filepath"); ImGui::SameLine();
		ImGui::SetCursorPosX(offsetFromMainText);
		ImGui::InputText("##ModelFilepath", &mModelFilepath);

		return !canClose;
	}
}