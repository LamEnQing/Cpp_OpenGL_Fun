#pragma once

namespace OpenGLSandbox {
	struct Vertex {
		float position[3];
		float color[4];
		float texCoords[2];
		float texID;

		Vertex(float posX, float posY, float posZ, float red, float green, float blue, float alpha, float u, float v, float texId) : position{ posX, posY, posZ }, color{ red, green, blue, alpha }, texCoords{ u, v }, texID{ texId } {}

		Vertex(float red, float green, float blue, float alpha, float u, float v, float texId) : position{ 0.0f, 0.0f, 0.0f }, color{ red, green, blue, alpha }, texCoords{ u, v }, texID{ texId } {}
	};

	// Apply transformations from data members into vertex
	struct VertexWrapper {
		float m_position[3]; // the vertex m_position before transformation, that is, in the range of -1 to 1
		Vertex vertex; // the position in the vertex is after transformation
	};
}