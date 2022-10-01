#include "Serializer.h"

namespace OpenGLFun {
	namespace Serializer {
		std::string GetFileContents(const char* filepath) {
			static std::fstream fs;
			fs.open(filepath, std::fstream::in);

			if (!fs.is_open())
				throw SimpleException(std::string("Failed to open ") + filepath);

			fs.seekg(0, fs.end); // move file cursor to end of file
			size_t fs_size = fs.tellg(); // get position of the file cursor, which can be assumed as the file's size
			fs.seekg(0, fs.beg); // move file cursor back to beginning of file

			std::string file_contents(fs_size, ' '); // set the string's contents with size fs_size and set those characters to whitespace
			fs.read(&file_contents[0], fs_size);
			fs.close();

			return file_contents;
		}

		bool DoesFilenameEndWith(std::string const& filename, std::string endStr) {
			if (endStr.size() > filename.size())
				return false;

			// say filename is 5 chars, and endStr is 4 chars, you should start at 1 which is 5 - 4.
			// say filename is 5 chars, and endStr is 4 chars, you should start at 1 which is 5 - 4.
			for (size_t i = 0; i < endStr.size(); i++) {
				if (filename[filename.size() - endStr.size() + i] !=
					endStr[i])
					return false;
			}
			return true;
		}
	}
}