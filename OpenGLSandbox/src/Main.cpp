#include "Main.h"

#define STB_IMAGE_IMPLEMENTATION
#define _CRTDBG_MAP_ALLOC

#include <stb/stb_image.h>

#include <chrono>
#include <crtdbg.h>
#include <exception>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "BatchRendering.h"
#include "GridsBatchRendering.h"
#include "OrthoTest.h"
#include "StarSystem.h"
#include "ThreadDrawing.h"
#include "UVTest.h"
#include "Vertex.h"

GLFWwindow* windowPtr = nullptr;
OpenGLSandbox::ShaderProgram batchTexShdrPgm;
OpenGLSandbox::ShaderProgram batchColorShdrPgm;
OpenGLSandbox::ShaderProgram posColorShdrPgm;

void WindowCreation() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // can resize or not?

	windowPtr = glfwCreateWindow(static_cast<int>(OpenGLSandbox::SCREEN_WIDTH), static_cast<int>(OpenGLSandbox::SCREEN_HEIGHT), "OpenGL Sandbox", NULL, NULL);
	if (windowPtr == nullptr) {
		throw std::exception("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(windowPtr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::exception("Failed to initialize GLAD");
	}
}

void MyImGuiInit() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.Fonts->AddFontFromFileTTF("./assets/fonts/Roboto-Regular.ttf", 16.0f);

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void MyImGuiShutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ShadersInit() {
	if (!batchTexShdrPgm.CompileAndLink("batch_tex")) {
		batchTexShdrPgm.Destroy();
		throw std::exception("Failed to compile 'batch_tex' shaders.\n");
	}

	if (!batchColorShdrPgm.CompileAndLink("batch_color")) {
		batchColorShdrPgm.Destroy();
		throw std::exception("Failed to compile 'batch_color' shaders.\n");
	}

	if (!posColorShdrPgm.CompileAndLink("pos_color")) {
		posColorShdrPgm.Destroy();
		throw std::exception("Failed to compile 'pos_color' shaders.\n");
	}
}

int main(void) {
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(175);
	#endif

	try {
		//stbi_set_flip_vertically_on_load(1);

		WindowCreation();
		ShadersInit();
		MyImGuiInit();

		//OpenGLSandbox::BatchRendering batchRender;
		//OpenGLSandbox::GridsBatchRendering gridBatchRender;
		//OpenGLSandbox::OrthoTest orthoTest;
		//OpenGLSandbox::StarSystem starSystem;
		OpenGLSandbox::ThreadDrawing threadDrawTest;
		//OpenGLSandbox::UVTest uvTest;

		using clock = std::chrono::high_resolution_clock;

		auto startTime = clock::now();
		auto lastTime = startTime;
		int frameCount = 0;
		float timeElapsed = 0.0f;

		while (!glfwWindowShouldClose(windowPtr)) {
			//glViewport(0.0f, 0.0f, OpenGLSandbox::SCREEN_WIDTH, OpenGLSandbox::SCREEN_HEIGHT);
			auto currTime = clock::now();
			auto deltaTime = currTime - lastTime;
			lastTime = clock::now();

			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//ImGui::ShowDemoWindow(NULL);

			// Drawing codes
			//batchRender.Draw(batchTexShdrPgm);
			//gridBatchRender.Draw(batchColorShdrPgm);
			//orthoTest.Draw();
			//starSystem.Draw();
			threadDrawTest.Draw();
			//uvTest.Draw();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			glfwSwapBuffers(windowPtr);
		}
	}
	catch (const std::exception& e) {
		std::cout << "\nEncountered an error:\n"
			<< "------------------------------------------------------------------------\n"
			<< e.what() << '\n';
		std::cout << "------------------------------------------------------------------------\n";
	}

	MyImGuiShutdown();
	glfwTerminate();

	return 0;
}