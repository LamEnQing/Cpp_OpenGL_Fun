#pragma once
#include "pch.h"
#include "Component.h"

namespace OpenGLFun {
	class Camera : public IComponent {
	public:
		glm::vec3 mCamOffset; // offset from player position
		glm::vec3 mLookAtPrev;
		glm::vec3 mLookAt; // where is the camera looking at?
		glm::vec3 mCamUp; // up direction of the camera, to orient the camera

		Camera() : IComponent(), mCamOffset{}, mLookAtPrev{}, mLookAt{}, mCamUp{} {
			mCompType = ComponentType::Camera;
			std::cout << "Camera constructor\n";
		}

		Camera(EntityId const& owner, glm::vec3 camOffset, glm::vec3 lookAt, glm::vec3 camUp) : Camera() {
			mOwner = owner;
			mCamOffset = camOffset;
			mLookAt = lookAt;
			mCamUp = camUp;
		}

		~Camera() {}
		void Deserialize(rapidjson::Value const&) override {}
	};
}