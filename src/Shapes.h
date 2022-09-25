#pragma once
#include "Vertex.h"
#include <vector>

namespace OpenGLFun {
	/**
	*/
	struct Quad {
		Quad& init(Vertex vertexOne, Vertex vertexTwo, Vertex vertexThree, Vertex vertexFour);

		const std::vector<Vertex>& Vertices() const;
		const size_t& VertexCount() const;

		private:
			std::vector<Vertex> vertices{};
	};
}