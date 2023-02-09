/*
Created by Lam En Qing.

Description:
	OrthoTest is an experiment with the Orthographic projection, using glm::ortho function to generate an Orthographic projection matrix.
	The experiment is to look at scaling/zooming, positioning and rotation.
	Additionally, the content being drawn should maintain it's size or be cutoff/trimmed when the window is resized.
*/
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GlobalDeclared.h"
#include "ShaderProgram.h"

namespace OpenGLSandbox {
	class OrthoTest {
	private:
		float zoom{ 1.0f };
		glm::vec2 position{ 0.0f, 0.0f };
		float rotation{ 0.0 };

		GLuint vao{ uint_max }, vbo{ uint_max }, ibo{ uint_max };
		GLuint myTex{ uint_max };

		ShaderProgram myShdrPgm;

		void ImGuiControls();
	public:
		OrthoTest();
		~OrthoTest();

		void Draw();
	};
}