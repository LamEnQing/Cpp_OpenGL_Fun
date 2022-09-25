#include "Shapes.h"

namespace OpenGLFun {
	Quad& Quad::init(Vertex vertexOne, Vertex vertexTwo, Vertex vertexThree, Vertex vertexFour) {
		this->vertices.clear();

		// since the vertices are in anti-clockwise order, the triangles would be: 123, 134
		this->vertices.push_back(vertexOne);
		this->vertices.push_back(vertexTwo);
		this->vertices.push_back(vertexThree);

		this->vertices.push_back(vertexOne);
		this->vertices.push_back(vertexThree);
		this->vertices.push_back(vertexFour);

		return *this;
	}

	const std::vector<Vertex>& Quad::Vertices() const {
		return this->vertices;
	}

	const size_t& Quad::VertexCount() const { return this->vertices.size(); }
}