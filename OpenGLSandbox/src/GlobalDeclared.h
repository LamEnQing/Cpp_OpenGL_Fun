#pragma once

#include <cstdint>
#include <numeric>

namespace OpenGLSandbox {
	using VertexIndexType = uint32_t;
	
	static int SCREEN_WIDTH = 854, SCREEN_HEIGHT = 480;

	static unsigned int uint_max = std::numeric_limits<unsigned int>::max();

}