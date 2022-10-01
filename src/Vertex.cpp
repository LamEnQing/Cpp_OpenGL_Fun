#include "Vertex.h"

namespace OpenGLFun {
	Vertex& Vertex::Pos(float x, float y, float z) {
		this->mPos = { x, y, z };

		return *this;
	}

	Vertex& Vertex::Color(float red, float green, float blue, float alpha) {
		this->mColor = { red, green, blue, alpha };

		return *this;
	}

	Vertex& Vertex::UV(float u, float v) {
		this->mUV = { u, v };

		return *this;
	}
}