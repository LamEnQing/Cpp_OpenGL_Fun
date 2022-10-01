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

		Camera() : IComponent(), mCamOffset{}, mLookAtPrev{}, mLookAt{}, mCamUp{} {
			mCompType = ComponentType::Camera;
			std::cout << "Camera constructor\n";
		}

		Camera(EntityId const& owner, Vec3f camOffset, Vec3f lookAt, Vec3f camUp) : Camera() {
			mOwner = owner;
			mCamOffset = camOffset;
			mLookAt = lookAt;
			mCamUp = camUp;
		}

		~Camera() {}
		void Deserialize(rapidjson::Value const&) override {}
	};
}