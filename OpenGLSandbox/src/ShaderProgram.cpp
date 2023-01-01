#include "ShaderProgram.h"

#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace OpenGLSandbox {
	unsigned int currShdrPgm = 0;

	std::string MakeShaderPath(std::string const& shaderName, std::string const& fileExt) {
		return std::string("assets/shaders/") + shaderName + fileExt;
	}

	bool ShaderProgram::CompileAndLink(const std::string& shaderName) {
		OpenGLSandbox::Shader vertexShader{}, fragmentShader{};

		if (!vertexShader.Compile(OpenGLSandbox::ShaderType::Vertex, MakeShaderPath(shaderName, ".vert"))) {
			vertexShader.Destroy();

			throw std::exception((std::string("Failed to compile ") + MakeShaderPath(shaderName, ".vert") + "\n").c_str());
		}
		if (!fragmentShader.Compile(OpenGLSandbox::ShaderType::Fragment, MakeShaderPath(shaderName, ".frag"))) {
			fragmentShader.Destroy();

			throw std::exception((std::string("Failed to compile ") + MakeShaderPath(shaderName, ".frag") + "\n").c_str());
		}

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

			std::cout << "Shader '" << shaderName << "' linking failed: \n\t" << infoLog.data();
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
		currShdrPgm = mProgramId;
	}
}