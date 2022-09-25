#pragma once
#include <string>

namespace OpenGLFun {
	struct Texture {
		unsigned int ID;

		bool create(std::string imageFilepath);
	};
}