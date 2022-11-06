#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <windows.h>
#include "Engine.h"

namespace OpenGLFun {
	Engine* ENGINE = nullptr;
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
int main() {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(217);
	::ShowWindow(::GetConsoleWindow(), true);
#endif
	/*UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);*/

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