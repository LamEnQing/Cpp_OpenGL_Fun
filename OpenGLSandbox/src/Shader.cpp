#include "Shader.h"

#include <glad/glad.h>
#include <vector>
#include <iostream>

#include "FileOps.h"

namespace OpenGLSandbox {
	bool Shader::Compile(ShaderType type, std::string shaderFilepath) {
		std::string shaderSrc;
		try {
			shaderSrc = Serializer::GetFileContents(shaderFilepath.c_str());
		}
		catch (...) {
			std::cout << "Failed to read shader: " << shaderFilepath.c_str() << '\n';
			return false;
		}

		GLenum shaderType = GL_VERTEX_SHADER;
		if (type == ShaderType::Fragment)
			shaderType = GL_FRAGMENT_SHADER;
		mShaderId = glCreateShader(shaderType);

		const GLchar* shaderSrcGL = shaderSrc.c_str();
		glShaderSource(mShaderId, 1, &shaderSrcGL, NULL);

		glCompileShader(mShaderId);
		int isCompiled;
		glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &isCompiled);

		if (!isCompiled) {
			// If compilation failed, find out why and log the error
			GLint maxLength = 0;
			glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(mShaderId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore if compilation failed
			glDeleteShader(mShaderId);

			std::cout << "Shader Compilation failed: \n" << infoLog.data();

			mShaderId = UINT32_MAX;
			return false;
		}

		return true;
	}

	void Shader::Destroy() {
		if (mShaderId != UINT32_MAX) {
			glDeleteShader(mShaderId);
			mShaderId = UINT32_MAX;
		}
	}
}