#include "FunImGuiSystem.h"

#include "pch.h"
#include "WindowSystem.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace OpenGLFun {
	FunImGuiSystem* FUN_IMGUI_SYSTEM = nullptr;

	FunImGuiSystem::FunImGuiSystem() : mShowDemoWindow{ false } {
		if (FUN_IMGUI_SYSTEM != nullptr)
			throw SimpleException("FunImGuiSystem has already been created!");

		FUN_IMGUI_SYSTEM = this;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(WINDOW_SYSTEM->mWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	FunImGuiSystem::~FunImGuiSystem() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void FunImGuiSystem::Update(float const& deltaTime) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (mShowDemoWindow)
			ImGui::ShowDemoWindow(&mShowDemoWindow);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}