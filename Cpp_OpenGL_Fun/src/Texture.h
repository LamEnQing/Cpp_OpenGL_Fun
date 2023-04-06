#pragma once
#include "pch.h"

namespace OpenGLFun {
	struct Texture {
		unsigned int mGLTextureId{ 0 };
		int imgWidth{ 0 }, imgHeight{ 0 }, imgChannels{ 0 };
	};
}