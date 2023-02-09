#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

extern GLFWwindow* windowPtr;
extern OpenGLSandbox::ShaderProgram batchTexShdrPgm;
extern OpenGLSandbox::ShaderProgram batchColorShdrPgm;
extern OpenGLSandbox::ShaderProgram posColorShdrPgm;