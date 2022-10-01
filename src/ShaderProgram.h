#pragma once
#include "Shader.h"

namespace OpenGLFun {
	struct ShaderProgram {
		unsigned int mProgramId;

		bool CompileAndLink(Shader& vertexShader, Shader& fragmentShader);
		void Destroy(void);

		void use(void);
	};
}