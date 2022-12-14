#define STB_IMAGE_IMPLEMENTATION
#define _CRTDBG_MAP_ALLOC

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <crtdbg.h>
#include <exception>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "BatchRendering.h"
#include "ShaderProgram.h"
#include "Vertex.h"

GLFWwindow* windowPtr = nullptr;
OpenGLSandbox::ShaderProgram batchShdrPgm;

void WindowCreation() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // can resize or not?

	windowPtr = glfwCreateWindow(1280, 720, "OpenGL Sandbox", NULL, NULL);
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
	if (!batchShdrPgm.CompileAndLink("assets/shaders/batch.vert", "assets/shaders/batch.frag")) {
		batchShdrPgm.Destroy();
		throw std::exception("Failed to compile 2D shader program.\n");
	}
}

int main(void) {
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(175);
	#endif

	try {
		stbi_set_flip_vertically_on_load(1);

		WindowCreation();
		ShadersInit();
		MyImGuiInit();

		BatchRendering batchRender;

		while (!glfwWindowShouldClose(windowPtr)) {
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Drawing codes
			batchRender.Draw(batchShdrPgm);

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

		MyImGuiShutdown();
	}
	catch (const std::exception& e) {
		std::cout << "\nEncountered an error:\n"
			<< "------------------------------------------------------------------------\n"
			<< e.what() << '\n';
		std::cout << "------------------------------------------------------------------------\n";
	}

	glfwTerminate();

	return 0;
}