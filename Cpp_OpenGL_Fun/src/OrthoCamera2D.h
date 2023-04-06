#pragma once

#include "pch.h"
#include "ICamera.h"
#include "Vec3f.h"

namespace OpenGLFun {
	class OrthoCamera : public ICamera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		const Vec3f& GetPosition() const override;
		void SetPosition(const Vec3f& position) override;

		float GetRotation() const override;
		void SetRotation(float rotation) override;

		const glm::mat4& GetProjMatrix() const override;
		const glm::mat4& GetViewMatrix() const override;
		const glm::mat4& GetViewProjMatrix() const override;
	private:
		void RecalculateViewMatrix() override;
	private:
		glm::mat4 _projMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjMatrix;

		Vec3f _position;
		float _rotation;
	};
}