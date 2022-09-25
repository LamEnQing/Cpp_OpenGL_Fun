#pragma once
#include "pch.h"

namespace OpenGLFun {
	enum class ShaderType {
		Vertex,
		Fragment
	};

	struct Shader {
		unsigned int shaderId;
		ShaderType type;

		// Create a shader and read from a shader file
		bool Compile(ShaderType type, std::string shaderFilepath);
		void Destroy(void);
	};
}