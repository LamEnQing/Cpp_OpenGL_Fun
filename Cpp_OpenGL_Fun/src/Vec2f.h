#pragma once

namespace OpenGLFun {
	#ifdef _MSC_VER
		// Supress warning: nonstandard extension used : nameless struct/union
	#pragma warning( disable : 4201 )
	#endif

	union Vec2f {
		float data[2];
		struct {
			float x, y;
		};

		Vec2f();
		Vec2f(float _x, float _y);

		Vec2f& operator*=(const float& obj);

		const float& operator[](size_t idx) const;
		float& operator[](size_t idx);
	};
}