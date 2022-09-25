#include "pch.h"

#include "ShaderProgram.h"

namespace OpenGLFun {
	bool ShaderProgram::CompileAndLink(Shader& vertexShader, Shader& fragmentShader) {
		programId = glCreateProgram();

		glAttachShader(programId, vertexShader.shaderId);
		glAttachShader(programId, fragmentShader.shaderId);

		glLinkProgram(programId);
		int isLinked;
		glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);

		if (!isLinked) {
			GLint maxLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore if linking failed
			glDeleteProgram(programId);
			vertexShader.Destroy();
			fragmentShader.Destroy();

			std::cout << "Shader linking failed: \n\t" << infoLog.data();
			programId = UINT32_MAX;
			return false;
		}

		// Since it's a successful link, we can detach and destroy the shaders
		glDetachShader(programId, vertexShader.shaderId);
		glDetachShader(programId, fragmentShader.shaderId);
		vertexShader.Destroy();
		fragmentShader.Destroy();

		return true;
	}

	void ShaderProgram::Destroy() {
		if (programId != UINT32_MAX) {
			glDeleteProgram(programId);
			programId = UINT32_MAX;
		}
	}

	void ShaderProgram::use() {
		glUseProgram(programId);
	}
}