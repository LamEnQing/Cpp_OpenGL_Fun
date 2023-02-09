#pragma once

#include <glad/glad.h>
#include "GlobalDeclared.h"
#include "ShaderProgram.h"

namespace OpenGLSandbox {
	class UVTest {
	private:
		float uvOffset[2];
		float uvSize[2];
		int imgWidth{ 0 }, imgHeight{ 0 };

		GLuint vao{ uint_max }, vbo{ uint_max }, ibo{ uint_max };
		GLuint myTex{ uint_max };
		ShaderProgram mainShdr;

		struct MyVertex {
			float position[2];
			float uv[2];

			MyVertex(float posX, float posY, float u, float v) : position{ posX, posY }, uv{ u, v } {}
		};
	public:
		UVTest();
		~UVTest();

		void Draw();
	};
}