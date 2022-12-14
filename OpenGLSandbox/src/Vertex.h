#pragma once

namespace OpenGLSandbox {
	struct Vertex {
		float position[3];
		float color[4];
		float texCoords[2];
		float texID;

		Vertex(float posX, float posY, float posZ, float red, float green, float blue, float alpha, float u, float v, float texId) : position{ posX, posY, posZ }, color{ red, green, blue, alpha }, texCoords{ u, v }, texID{ texId } {}
	};
}