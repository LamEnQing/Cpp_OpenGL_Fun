#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec3f.h"

namespace OpenGLFun {
	class Camera : public IComponent {
	public:
		Vec3f mCamOffset; // offset from player position
		Vec3f mLookAtPrev;
		Vec3f mLookAt; // where is the camera looking at?
		Vec3f mCamUp; // up direction of the camera, to orient the camera
		Vec3f mCamRotation;

		Camera() : IComponent(), mCamOffset{}, mLookAtPrev{}, mLookAt{}, mCamUp{}, mCamRotation{} {
			mCompType = ComponentType::Camera;
		}

		Camera(EntityId const& owner, Vec3f camOffset, Vec3f lookAt, Vec3f camUp) : Camera() {
			mOwner = owner;
			mCamOffset = camOffset;
			mLookAt = lookAt;
			mCamUp = camUp;
		}

		~Camera() {}
		void Deserialize(rapidjson::Value const& jsonObj) override {
			if (!jsonObj.HasMember("offset") || !jsonObj["offset"].IsArray() || jsonObj["offset"].Size() < 3)
				throw SimpleException("Component of type Camera must have key 'offset' with an array of size 3");

			const rapidjson::Value& offsetArr = jsonObj["offset"];
			for (int i = 0; i < 3; i++) {
				if (!offsetArr[i].IsNumber())
					throw JsonReadException("component of type Camera", std::string("offset[") + std::to_string(i) + "]", "float");

				mCamOffset[i] = offsetArr[i].GetFloat();
			}

			if (!jsonObj.HasMember("look_at") || !jsonObj["look_at"].IsArray() || jsonObj["look_at"].Size() < 3)
				throw SimpleException("Component of type Camera must have key 'look_at' with an array of size 3");

			const rapidjson::Value& lookAtArr = jsonObj["look_at"];
			for (int i = 0; i < 3; i++) {
				if (!lookAtArr[i].IsNumber())
					throw JsonReadException("component of type Camera", std::string("look_at[") + std::to_string(i) + "]", "float");

				mLookAt[i] = lookAtArr[i].GetFloat();
			}

			if (!jsonObj.HasMember("up_direction") || !jsonObj["up_direction"].IsArray() || jsonObj["up_direction"].Size() < 3)
				throw SimpleException("Component of type Camera must have key 'up_direction' with an array of size 3");

			const rapidjson::Value& upDirArr = jsonObj["up_direction"];
			for (int i = 0; i < 3; i++) {
				if (!upDirArr[i].IsNumber())
					throw JsonReadException("component of type Camera", std::string("up_direction[") + std::to_string(i) + "]", "float");

				mCamUp[i] = upDirArr[i].GetFloat();
			}
		}
	};
}