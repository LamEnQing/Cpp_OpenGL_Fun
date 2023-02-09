#pragma once

#include <glad/glad.h>
#include <string>

namespace OpenGLSandbox {
	GLuint LoadTexture(std::string path, int* imgWidth = nullptr, int* imgHeight = nullptr);
}