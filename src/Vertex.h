#pragma once
#include "pch.h"
#include "Vec3f.h"
#include "Vec4f.h"

namespace OpenGLFun {
	struct Vertex {
		//glm::vec2 uv;
		Vec3f mPos{ 0.0f, 0.0f, 0.0f };
		Vec4f mColor{ 1.0f, 1.0f, 1.0f, 1.0f };

		Vertex& Pos(float x, float y, float z);

		Vertex& Color(float red, float green, float blue, float alpha=1.0f);

	};
}