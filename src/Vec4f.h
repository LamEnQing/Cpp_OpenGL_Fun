#pragma once

namespace OpenGLFun {
	union Vec4f {
		float data[4];
		struct {
			float x, y, z, w;
		};

		Vec4f();
		Vec4f(float v);
		Vec4f(float _x, float _y, float _z, float _w);

		const float& operator[](size_t idx) const;
		float& operator[](size_t idx);
	};
}