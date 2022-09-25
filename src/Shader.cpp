#include "Shader.h"
#include "Serializer.h"

namespace OpenGLFun {
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
		shaderId = glCreateShader(shaderType);

		const GLchar* shaderSrcGL = shaderSrc.c_str();
		glShaderSource(shaderId, 1, &shaderSrcGL, NULL);

		glCompileShader(shaderId);
		int isCompiled;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

		if (!isCompiled) {
			// If compilation failed, find out why and log the error
			GLint maxLength = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore if compilation failed
			glDeleteShader(shaderId);

			std::cout<< "Shader Compilation failed: \n" << infoLog.data();

			shaderId = UINT32_MAX;
			return false;
		}

		return true;
	}

	void Shader::Destroy() {
		if (shaderId != UINT32_MAX) {
			glDeleteShader(shaderId);
			shaderId = UINT32_MAX;
		}
	}
}