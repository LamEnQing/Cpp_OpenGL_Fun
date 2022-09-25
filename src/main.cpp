#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "main.h"

namespace OpenGLFun {
	Engine* engine;
}

int main() {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3254);
#endif

	OpenGLFun::engine = nullptr;
	try {
		OpenGLFun::engine = new OpenGLFun::Engine();
		OpenGLFun::engine->GameLoop();
	} catch (const std::exception& e) {
		std::cout << "Encountered an error with the Engine:\n\t" << e.what()<< '\n';
	}

	if (OpenGLFun::engine != nullptr)
		delete OpenGLFun::engine;

	/*std::fstream fs;
	fs.open("assets/test.json", std::fstream::in);

	if (!fs.is_open()) {
		std::cout << "couldn't open file";
		return 1;
	}

	fs.seekg(0, fs.end); // move file cursor to end of file
	size_t fs_size = fs.tellg(); // get position of the file cursor, which can be assumed as the file's size
	fs.seekg(0, fs.beg); // move file cursor back to beginning of file
	std::cout << "file size:" << fs_size << '\n';

	std::string json_from_file(fs_size, ' ');
	fs.read(&json_from_file[0], fs_size);

	const char* json = "{ \"name\":\"Hello World\", \"type\":\"experiment\" }";
	rapidjson::Document d;
	d.Parse(json_from_file.c_str());

	std::cout << "Json from file:\n" << json_from_file << '\n';
	std::cout << "d['name']:" << d["name"].GetString() << '\n';*/

	return 0;
}