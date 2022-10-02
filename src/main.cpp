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
	//_CrtSetBreakAlloc(1012);
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

	return 0;
}