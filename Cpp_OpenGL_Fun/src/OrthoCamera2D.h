#pragma once

#include "pch.h"
#include "Vec3f.h"

namespace OpenGLFun {
	class OrthoCamera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		const Vec3f& GetPosition() const;
		void SetPosition(const Vec3f& position);

		float GetRotation() const;
		void SetRotation(float rotation);

		const glm::mat4& GetProjMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewProjMatrix() const;
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 _projMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjMatrix;

		Vec3f _position;
		float _rotation;
	};
}