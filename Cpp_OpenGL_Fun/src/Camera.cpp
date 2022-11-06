#include "Camera.h"

namespace OpenGLFun {
	Camera::Camera() : IComponent(), mEyeHeight{ 1.0f }, mCamOffset{}, mLookAtPrev{}, mLookAt{}, mCamUp{}, mCamRotation{} {
		mCompType = ComponentType::Camera;
	}

	Camera::Camera(EntityId const& owner, Vec2f camOffset, Vec3f lookAt, Vec3f camUp) : Camera() {
		mOwner = owner;
		mCamOffset = camOffset;
		mLookAt = lookAt;
		mCamUp = camUp;
	}

	Camera::~Camera() {}

	void Camera::Deserialize(rapidjson::Value const& jsonObj) {
		if (jsonObj.HasMember("eye_height")) {
			if (!jsonObj["eye_height"].IsNumber())
				throw JsonReadException("component of type Camera", std::string("eye_height"), "number");

			mEyeHeight = jsonObj["eye_height"].GetFloat();
		}

		if (!jsonObj.HasMember("xz_offset") || !jsonObj["xz_offset"].IsArray() || jsonObj["xz_offset"].Size() < 2)
			throw SimpleException("Component of type Camera must have key 'xz_offset' with an array of size 2");

		const rapidjson::Value& offsetArr = jsonObj["xz_offset"];
		for (int i = 0; i < 2; i++) {
			if (!offsetArr[i].IsNumber())
				throw JsonReadException("component of type Camera", std::string("xz_offset[") + std::to_string(i) + "]", "number");

			mCamOffset[i] = offsetArr[i].GetFloat();
		}

		if (!jsonObj.HasMember("look_at") || !jsonObj["look_at"].IsArray() || jsonObj["look_at"].Size() < 3)
			throw SimpleException("Component of type Camera must have key 'look_at' with an array of size 3");

		const rapidjson::Value& lookAtArr = jsonObj["look_at"];
		for (int i = 0; i < 3; i++) {
			if (!lookAtArr[i].IsNumber())
				throw JsonReadException("component of type Camera", std::string("look_at[") + std::to_string(i) + "]", "number");

			mLookAt[i] = lookAtArr[i].GetFloat();
		}

		if (!jsonObj.HasMember("up_direction") || !jsonObj["up_direction"].IsArray() || jsonObj["up_direction"].Size() < 3)
			throw SimpleException("Component of type Camera must have key 'up_direction' with an array of size 3");

		const rapidjson::Value& upDirArr = jsonObj["up_direction"];
		for (int i = 0; i < 3; i++) {
			if (!upDirArr[i].IsNumber())
				throw JsonReadException("component of type Camera", std::string("up_direction[") + std::to_string(i) + "]", "number");

			mCamUp[i] = upDirArr[i].GetFloat();
		}
	}

	bool Camera::DrawImGuiComponent() {
		bool canClose = true;
		if (!ImGui::CollapsingHeader("Camera", &canClose)) return !canClose;
		return !canClose;
	}
}