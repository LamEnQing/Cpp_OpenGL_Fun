#pragma once
#include "Shader.h"

namespace OpenGLFun {
	struct ShaderProgram {
		unsigned int mProgramId{ UINT32_MAX };

		bool CompileAndLink(Shader& vertexShader, Shader& fragmentShader);
		void Destroy(void);

		void use(void);
	};
}