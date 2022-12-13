#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <windows.h>
#include "Engine.h"

namespace OpenGLFun {
	Engine* ENGINE = nullptr;
}

int main() {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(217);
	::ShowWindow(::GetConsoleWindow(), true);
	//BOOL IsSuccess = AllocConsole();
	//std::cout << "Console Result:" << IsSuccess << std::endl;
#else
	::ShowWindow(::GetConsoleWindow(), false);
#endif

	try {
		OpenGLFun::ENGINE = new OpenGLFun::Engine();
		OpenGLFun::ENGINE->GameLoop();
	} catch (const std::exception& e) {
		std::cout << "\nEncountered an error with the Engine:\n"
			<< "------------------------------------------------------------------------\n"
			<< e.what()<< '\n';
		std::cout << "------------------------------------------------------------------------\n";
	}

	if (OpenGLFun::ENGINE != nullptr)
		delete OpenGLFun::ENGINE;

	return 0;
}