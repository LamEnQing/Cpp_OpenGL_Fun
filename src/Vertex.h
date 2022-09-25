#pragma once
#include <glm/glm.hpp>

namespace OpenGLFun {
	struct Vertex {
		glm::vec3 pos{ 0.0f, 0.0f, 0.0f };
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		//glm::vec2 uv;

		Vertex& Pos(float x, float y, float z);

		Vertex& Color(float red, float green, float blue, float alpha=1.0f);
	};
}