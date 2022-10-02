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
	//_CrtSetBreakAlloc(217);
#endif

	OpenGLFun::engine = nullptr;
	try {
		OpenGLFun::engine = new OpenGLFun::Engine();
		OpenGLFun::engine->GameLoop();
	} catch (const std::exception& e) {
		std::cout << "\nEncountered an error with the Engine:\n"
			<< "------------------------------------------------------------------------\n"
			<< e.what()<< '\n';
		std::cout << "------------------------------------------------------------------------\n";
	}

	if (OpenGLFun::engine != nullptr)
		delete OpenGLFun::engine;

	return 0;
}