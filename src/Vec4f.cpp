#include "Vec4f.h"

namespace OpenGLFun {
	Vec4f::Vec4f() : data{ 0.0f, 0.0f, 0.0f, 0.0f } {}
	Vec4f::Vec4f(float v) : data{ v, v, v, v } {}
	Vec4f::Vec4f(float _x, float _y, float _z, float _w) : data{ _x, _y, _z, _w } {}

	const float& Vec4f::operator[](size_t idx) const { return data[idx]; }
	float& Vec4f::operator[](size_t idx) { return data[idx]; }
}