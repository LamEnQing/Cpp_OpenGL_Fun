#pragma once
#include "Shader.h"

namespace OpenGLSandbox {
	struct ShaderProgram {
		unsigned int mProgramId{ UINT32_MAX };

		bool CompileAndLink(const std::string& shaderName);
		void Destroy(void);

		void use(void);
	};
}