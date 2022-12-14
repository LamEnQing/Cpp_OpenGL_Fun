#include "FileOps.h"

#include <exception>
#include <fstream>

namespace OpenGLSandbox {
	namespace Serializer {
		std::string GetFileContents(const char* filepath) {
			static std::fstream fs;
			fs.open(filepath, std::fstream::in);

			if (!fs.is_open())
				throw std::exception((std::string("Failed to open ") + filepath).c_str());

			fs.seekg(0, fs.end); // move file cursor to end of file
			size_t fs_size = fs.tellg(); // get position of the file cursor, which can be assumed as the file's size
			fs.seekg(0, fs.beg); // move file cursor back to beginning of file

			std::string file_contents(fs_size, ' '); // set the string's contents with size fs_size and set those characters to whitespace
			fs.read(&file_contents[0], fs_size);
			fs.close();

			return file_contents;
		}
	}
}