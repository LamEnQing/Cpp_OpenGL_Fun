#pragma once

#include "pch.h"

namespace OpenGLFun {
	namespace StringUtils {
		std::string ToLower(std::string str);

		bool DoesStringEndWith(std::string const& filename, std::string endStr);
		bool DoesStringStartWith(std::string const& filename, std::string startStr);
	}
}