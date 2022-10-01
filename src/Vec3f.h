#pragma once
#include "pch.h"
#include "Vec2f.h"

namespace OpenGLFun {
	union Vec3f {
		float data[3];
		struct {
			float x, y, z;
		};

		Vec3f();
		Vec3f(float v);
		Vec3f(float _x, float _y, float _z);

		Vec3f& operator+=(const Vec3f& rhs);
		Vec3f& operator+=(const Vec2f& rhs);

		Vec3f& operator-=(const Vec3f& rhs);

		const float& operator[](size_t idx) const;
		float& operator[](size_t idx);

		//Vec3f& operator=(const glm::vec3& rhs);
		bool operator!=(const Vec3f& rhs);
	};

	Vec3f operator+(const Vec3f& lhs, const Vec3f& rhs);
	Vec3f operator+(const Vec3f& lhs, const float& rhs);

	Vec3f operator*(const Vec3f& lhs, const float& rhs);

	Vec3f normalize(const Vec3f& obj);
	Vec3f cross(const Vec3f& lhs, const Vec3f& rhs);

	std::ostream& operator<<(std::ostream& os, const Vec3f& obj);
	glm::vec3 vec3f_to_vec3(const Vec3f& obj);
}