#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec3f.h"

namespace OpenGLFun {
	class Camera : public IComponent {
	public:
		float mEyeHeight; // Y offset from player's feet (player position is at player's feet)
		Vec2f mCamOffset; // XZ offset from player position
		Vec3f mLookAtPrev;
		Vec3f mLookAt; // where is the camera looking at?
		Vec3f mCamUp; // up direction of the camera, to orient the camera
		Vec3f mCamRotation;

		Camera();
		Camera(EntityId const& owner, Vec2f camOffset, Vec3f lookAt, Vec3f camUp);
		~Camera() override;

		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}