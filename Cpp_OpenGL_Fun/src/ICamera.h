#pragma once

#include "Vec3f.h"

namespace OpenGLFun {
	class ICamera {
	public:
		virtual void Zoom(float zoom) {
			this->mZoom += zoom * 0.01f;

			RecalculateViewMatrix();
		}

		virtual const Vec3f& GetPosition() const = 0;
		virtual void SetPosition(const Vec3f& position) = 0;

		virtual float GetRotation() const = 0;
		virtual void SetRotation(float rotation) = 0;

		virtual const glm::mat4& GetProjMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjMatrix() const = 0;
	protected:
		virtual void RecalculateViewMatrix() = 0;

		float mZoom = 1.0f;
	};
}