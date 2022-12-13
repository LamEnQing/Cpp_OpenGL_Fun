#include "OrthoCamera2D.h"

namespace OpenGLFun {
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
		: _projMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _viewMatrix(1.0f) {
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}

	const Vec3f& OrthoCamera::GetPosition() const { return _position; }
	void OrthoCamera::SetPosition(const Vec3f& pos) {
		_position = pos;
		RecalculateViewMatrix();
	}

	float OrthoCamera::GetRotation() const { return _rotation; }
	void OrthoCamera::SetRotation(float rotation) {
		_rotation = rotation;
		RecalculateViewMatrix();
	}

	const glm::mat4& OrthoCamera::GetProjMatrix() const { return _projMatrix; }
	const glm::mat4& OrthoCamera::GetViewMatrix() const { return _viewMatrix; }
	const glm::mat4& OrthoCamera::GetViewProjMatrix() const { return _viewProjMatrix; }

	void OrthoCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), vec3f_to_vec3(_position)) * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

		_viewMatrix = glm::inverse(transform);
		_viewProjMatrix = _projMatrix * _viewMatrix;
	}
}