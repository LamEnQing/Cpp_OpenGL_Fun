#pragma once
#include "pch.h"

namespace OpenGLFun {
	namespace Serializer {
		std::string GetFileContents(const char* filepath);

		bool DoesFilenameEndWith(std::string const& filename, std::string endStr);
	}
}