#pragma once
#include <glm/glm.hpp>

namespace OpenGLFun {
	struct Entity {
		glm::vec3 prevPos;
		glm::vec3 pos;
		float yRotOld; // y-rotation for the entire model
		float yRot;
	};
}