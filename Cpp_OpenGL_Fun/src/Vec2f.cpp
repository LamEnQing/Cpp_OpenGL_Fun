#include "Vec2f.h"

namespace OpenGLFun {
	Vec2f::Vec2f() : data{ 0.0f, 0.0f } {}
	Vec2f::Vec2f(float _x, float _y) : data{ _x, _y } {}

	Vec2f& Vec2f::operator*=(const float& obj) {
		this->x *= obj;
		this->y *= obj;
		return *this;
	}

	const float& Vec2f::operator[](size_t idx) const { return data[idx]; }
	float& Vec2f::operator[](size_t idx) { return data[idx]; }
}