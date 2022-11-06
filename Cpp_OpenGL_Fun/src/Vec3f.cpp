#include "Vec3f.h"
#include <cmath>

namespace OpenGLFun {
	Vec3f::Vec3f() : data{ 0.0f, 0.0f, 0.f } {}

	Vec3f::Vec3f(float v) : data{v, v, v} {}

	Vec3f::Vec3f(float _x, float _y, float _z) : data{ _x, _y, _z } {}

	Vec3f& Vec3f::operator+=(const Vec3f& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}
	Vec3f& Vec3f::operator+=(const Vec2f& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Vec3f& Vec3f::operator-=(const Vec3f& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	const float& Vec3f::operator[](size_t idx) const { return data[idx]; }
	float& Vec3f::operator[](size_t idx) { return data[idx]; }

	bool Vec3f::operator!=(const Vec3f& rhs) {
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

	Vec3f operator+(const Vec3f& lhs, const Vec3f& rhs) {
		Vec3f result(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		return result;
	}

	Vec3f operator+(const Vec3f& lhs, const float& rhs) {
		return Vec3f(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
	}

	Vec3f operator*(const Vec3f& lhs, const float& rhs) {
		return Vec3f(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}

	Vec3f normalize(const Vec3f& obj) {
		float length = std::sqrtf(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z);
		return Vec3f(obj.x / length, obj.y / length, obj.z / length);
	}
	Vec3f cross(const Vec3f& lhs, const Vec3f& rhs) {
		return Vec3f(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}

	std::ostream& operator<<(std::ostream& os, const Vec3f& obj) {
		os << "Vec3f:" << obj.x << ", " << obj.y << ", " << obj.z;
		return os;
	}

	glm::vec3 vec3f_to_vec3(const Vec3f& obj) {
		glm::vec3 v(1.0f);
		v.x = obj.x;
		v.y = obj.y;
		v.z = obj.z;
		return v;
	}
}