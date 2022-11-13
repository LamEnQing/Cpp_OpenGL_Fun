#include "StringUtils.h"

namespace OpenGLFun {
	namespace StringUtils {
		std::string ToLower(std::string str) {
			for (char& c : str) {
				c = static_cast<char>(std::tolower(c));
			}

			return str;
		}

		bool DoesStringEndWith(std::string const& str, std::string endStr) {
			if (endStr.size() > str.size())
				return false;

			// say str is 5 chars, and endStr is 4 chars, you should start at 1 which is 5 - 4.
			// say str is 5 chars, and endStr is 4 chars, you should start at 1 which is 5 - 4.
			for (size_t i = 0; i < endStr.size(); i++) {
				if (str[str.size() - endStr.size() + i] !=
					endStr[i])
					return false;
			}
			return true;
		}

		bool DoesStringStartWith(std::string const& str, std::string startStr) {
			if (startStr.size() > str.size()) return false;

			for (size_t i = 0; i < startStr.size(); i++)
				if (str[i] != startStr[i]) return false;

			return true;
		}
	}
}