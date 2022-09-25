#include "Vertex.h"

namespace OpenGLFun {
	Vertex& Vertex::Pos(float x, float y, float z) {
		this->pos = { x, y, z };

		return *this;
	}

	Vertex& Vertex::Color(float red, float green, float blue, float alpha) {
		this->color = { red, green, blue, alpha };

		return *this;
	}
}