#include "Transform.h"

#include "pch.h"
#include "ComponentManager.h"
#include "ModelComponent.h"
#include "WindowSystem.h"

namespace OpenGLFun {
	Transform::Transform() : IComponent(), mPositionOld{}, mPosition{}, mRotationOld{}, mRotation{}, mScale(1.0f) {
		mCompType = ComponentType::Transform;
	}
	Transform::Transform(EntityId const& owner, Vec3f pos, Vec3f rot, Vec3f scale) : Transform() {
		mOwner = owner;

		mPositionOld = mPosition = pos;
		mRotationOld = mRotation = rot;
		mScale = scale;
	}
	Transform::~Transform() {}

	void Transform::Deserialize(rapidjson::Value const& jsonObj) {
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

	bool Transform::DrawImGuiComponent() {
		bool canClose = true;
		if (!ImGui::CollapsingHeader("Transform", &canClose)) return !canClose;
		float offsetFromMainText = 80;

		static float posX = 0; posX = mPosition.x;
		static float posY = 0; posY = mPosition.y;
		static float posZ = 0; posZ = mPosition.z;
		static float rotationX = 0; rotationX = mRotation.x;
		static float rotationY = 0; rotationY = mRotation.y;
		static float rotationZ = 0; rotationZ = mRotation.z;
		static float scaleX = 0; scaleX = mScale.x;
		static float scaleY = 0; scaleY = mScale.y;
		static float scaleZ = 0; scaleZ = mScale.z;

		bool shouldClampFor2D = ENTITY_MANAGER->HasComponent(mOwner, ComponentType::Model) && COMPONENT_MANAGER->GetComponent<ModelComponent>(mOwner, ComponentType::Model)->mModelType == ModelType::TwoD;

		ImGui::PushItemWidth(100);
		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::SetCursorPosX(offsetFromMainText);
		ImGui::Text("X"); ImGui::SameLine();
		if (shouldClampFor2D)
			ImGui::SliderFloat("##TransPosX", &posX, -WINDOW_SYSTEM->mFrameWidth / 2.0f, WINDOW_SYSTEM->mFrameWidth / 2.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		else
			ImGui::InputFloat("##TransPosX", &posX);
		ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		if (shouldClampFor2D)
			ImGui::SliderFloat("##TransPosY", &posY, -WINDOW_SYSTEM->mFrameHeight / 2.0f, WINDOW_SYSTEM->mFrameHeight / 2.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		else
			ImGui::InputFloat("##TransPosY", &posY);
		if (!shouldClampFor2D) {
			ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::InputFloat("##trans pos z", &posZ, -1);
		}
		mPosition.x = posX;
		mPosition.y = posY;
		mPosition.z = shouldClampFor2D ? 0 : posZ;

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::SetCursorPosX(offsetFromMainText);
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::SliderFloat("##trans rotation x", &rotationX, -180, 180, "%.3f", ImGuiSliderFlags_AlwaysClamp); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::SliderFloat("##trans rotation y", &rotationY, -180, 180, "%.3f", ImGuiSliderFlags_AlwaysClamp); ImGui::SameLine();
		ImGui::Text("Z"); ImGui::SameLine();
		ImGui::SliderFloat("##trans rotation z", &rotationZ, -180, 180, "%.3f", ImGuiSliderFlags_AlwaysClamp);
		mRotation.x = rotationX;
		mRotation.y = rotationY;
		mRotation.z = rotationZ;

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::SetCursorPosX(offsetFromMainText);
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::InputFloat("##trans scale x", &scaleX, -1); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::InputFloat("##trans scale y", &scaleY, -1);
		if (!shouldClampFor2D) {
			ImGui::SameLine();
			ImGui::Text("Z"); ImGui::SameLine();
			ImGui::InputFloat("##trans scale z", &scaleZ, -1);
		}
		mScale.x = scaleX;
		mScale.y = scaleY;
		mScale.z = shouldClampFor2D ? 1.0f : scaleZ;

		ImGui::PopItemWidth();

		return !canClose;
	}
}