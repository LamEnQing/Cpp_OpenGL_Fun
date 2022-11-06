#include "pch.h"

#include "ShaderProgram.h"

namespace OpenGLFun {
	bool ShaderProgram::CompileAndLink(Shader& vertexShader, Shader& fragmentShader) {
		mProgramId = glCreateProgram();

		glAttachShader(mProgramId, vertexShader.mShaderId);
		glAttachShader(mProgramId, fragmentShader.mShaderId);

		glLinkProgram(mProgramId);
		int isLinked;
		glGetProgramiv(mProgramId, GL_LINK_STATUS, &isLinked);

		if (!isLinked) {
			GLint maxLength = 0;
			glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(mProgramId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore if linking failed
			glDeleteProgram(mProgramId);
			vertexShader.Destroy();
			fragmentShader.Destroy();

			std::cout << "Shader linking failed: \n\t" << infoLog.data();
			mProgramId = UINT32_MAX;
			return false;
		}

		// Since it's a successful link, we can detach and destroy the shaders
		glDetachShader(mProgramId, vertexShader.mShaderId);
		glDetachShader(mProgramId, fragmentShader.mShaderId);
		vertexShader.Destroy();
		fragmentShader.Destroy();

		return true;
	}

	void ShaderProgram::Destroy() {
		if (mProgramId != UINT32_MAX) {
			glDeleteProgram(mProgramId);
			mProgramId = UINT32_MAX;
		}
	}

	void ShaderProgram::use() {
		glUseProgram(mProgramId);
	}
}