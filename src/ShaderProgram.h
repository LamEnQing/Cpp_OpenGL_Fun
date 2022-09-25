#pragma once
#include "Shader.h"

namespace OpenGLFun {
	struct ShaderProgram {
		unsigned int programId;

		bool CompileAndLink(Shader& vertexShader, Shader& fragmentShader);
		void Destroy(void);

		void use(void);
	};
}