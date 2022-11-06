#pragma once

namespace OpenGLFun {
	#ifdef _MSC_VER
		// Supress warning: nonstandard extension used : nameless struct/union
	#pragma warning( disable : 4201 )
	#endif

	template<typename T>
	union Vec2 {
		T data[2];
		struct {
			T x, y;
		};

		Vec2();
		Vec2(T _x, T _y);

		Vec2<T>& operator*=(const T& obj);

		const T& operator[](size_t idx) const;
		T& operator[](size_t idx);
	};
	#include "Vec2.hpp"
}