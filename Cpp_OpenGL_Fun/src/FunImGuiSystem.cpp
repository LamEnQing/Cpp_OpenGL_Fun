#include "FunImGuiSystem.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "pch.h"
#include "ContentBrowserImgui.h"
#include "Engine.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "WindowSystem.h"

namespace OpenGLFun {
	FunImGuiSystem* FUN_IMGUI_SYSTEM = nullptr;
	int selectedEntity = -1;
	int selectedAddComponent = 0;
	int selectedLevel = -1;

	IComponent* deleteComp = nullptr; // the component to be deleted

	void DrawMenuBar();
	void DrawStats();
	void DrawEntityList();
	void DrawEntityProperty();
	void DrawGameScene();
	void DrawLoadLevelPopup();
	void DrawWarningPopup(const char* id, const char* message);
	void DrawWarningDeleteCompPopup(const char* compType);
	void DrawWarningDeleteEntityPopup();

	FunImGuiSystem::FunImGuiSystem() : mShowEditor{ false }, mSceneViewportSize{}, _showLevelSelect{ false } {
		if (FUN_IMGUI_SYSTEM != nullptr)
			throw SimpleException("FunImGuiSystem has already been created!");

		FUN_IMGUI_SYSTEM = this;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.Fonts->AddFontFromFileTTF("./assets/fonts/Roboto-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("./assets/fonts/DroidSans.ttf", 18.0f);

		ImGui::StyleColorsDark();
		ImGui::GetStyle().Alpha = 1.0f;

		ImGui_ImplGlfw_InitForOpenGL(WINDOW_SYSTEM->mWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	FunImGuiSystem::~FunImGuiSystem() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void FunImGuiSystem::Reset() {
		selectedEntity = -1;
		selectedAddComponent = 0;
		selectedLevel = -1;
	}

	void FunImGuiSystem::Update(float const& /*deltaTime*/) {
		if (mShowEditor) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuiIO& io = ImGui::GetIO();

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode, NULL);

			DrawMenuBar();
			DrawStats();
			DrawEntityList();
			DrawEntityProperty();
			DrawGameScene();
			_contentBrowser.Draw();

			DrawLoadLevelPopup();

			ImGui::ShowDemoWindow(NULL);

			ImGui::Render();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}
	}
	
	void DrawMenuBar() {
		bool shouldShowLevelSelect = false;
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("Load Level", NULL, &shouldShowLevelSelect);

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings")) {
				if (ImGui::BeginMenu("Editor")) {
					if (ImGui::MenuItem("Enabled", NULL, FUN_IMGUI_SYSTEM->mShowEditor))
						FUN_IMGUI_SYSTEM->mShowEditor = true;
					if (ImGui::MenuItem("Disabled", NULL, !FUN_IMGUI_SYSTEM->mShowEditor))
						FUN_IMGUI_SYSTEM->mShowEditor = false;
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (shouldShowLevelSelect) {
			std::cout << "Hello pop up should be opening\n";
			ImGui::OpenPopup("Load Level");
		}
	}

	void DrawStats() {
		ImGui::Begin("Statistics");

		ImGui::Text("Window Width:");
		ImGui::SameLine(); ImGui::SetCursorPosX(120); ImGui::Text(std::to_string(WINDOW_SYSTEM->GetWindowWidth()).c_str());
		ImGui::Text("Window Height:");
		ImGui::SameLine(); ImGui::SetCursorPosX(120); ImGui::Text(std::to_string(WINDOW_SYSTEM->GetWindowHeight()).c_str());

		ImGui::Text("Frame Width:");
		ImGui::SameLine(); ImGui::SetCursorPosX(110); ImGui::Text(std::to_string(WINDOW_SYSTEM->mFrameWidth).c_str());
		ImGui::Text("Frame Height:");
		ImGui::SameLine(); ImGui::SetCursorPosX(110); ImGui::Text(std::to_string(WINDOW_SYSTEM->mFrameHeight).c_str());

		ImGui::End();
	}

	void DrawEntityList() {
		ImGui::Begin("Entity List");

		static std::string listElementName{};
		ImGui::BeginListBox("##entity List", { -FLT_MIN, 8 * ImGui::GetTextLineHeightWithSpacing() });

		for (const EntityId& entityId : ENTITY_MANAGER->GetEntities()) {
			listElementName = "Entity #" + std::to_string(entityId);

			// adds c_str into the list
			if (ImGui::Selectable(listElementName.c_str(), selectedEntity == entityId))
				selectedEntity = entityId;

			if (selectedEntity == entityId)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndListBox();

		if (ImGui::Button("Add Entity", { ImGui::GetContentRegionAvailWidth(), 0 })) {
			ENTITY_MANAGER->SpawnEntity();
		}

		ImGui::End();
	}

	void DrawEntityProperty() {
		static std::map<ComponentType, const char*> componentTypeNames{
			{ ComponentType::Button, "Button" },
			//{ ComponentType::Camera, "Camera" },
			{ ComponentType::Color, "Color" },
			{ ComponentType::Model, "Model" },
			{ ComponentType::Sprite, "Sprite" },
			{ ComponentType::Transform, "Transform" }
		};

		bool shouldDisplayWarning = false;
		bool shouldDeleteEntity = false;
		bool shouldDisplayAddComp = false;

		ImGui::Begin("Entity Property", NULL, ImGuiWindowFlags_HorizontalScrollbar);
		if (selectedEntity == -1) {
			ImGui::Text("Select an entity!");
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, {0.7f, 0.1f, 0.1f, 1.0f});
			if (ImGui::Button("Delete Entity", { ImGui::GetContentRegionAvailWidth(), 0 })) {
				shouldDeleteEntity = true;
			}
			ImGui::PopStyleColor();

			if (ImGui::Button("Add Component")) {
				shouldDisplayAddComp = true;
			}
			
			for (IComponent* comp : COMPONENT_MANAGER->GetEntityComponents(selectedEntity)) {
				if (comp->DrawImGuiComponent()) {
					std::cout << "comp can close?" << std::endl;
					deleteComp = comp;
				}
			}
		}
		ImGui::End();

		if (shouldDisplayWarning)
			ImGui::OpenPopup("WARNING - Add Component");
		DrawWarningPopup("WARNING - Add Component", "Cannot add another of the same component!");

		if (deleteComp != nullptr)
			ImGui::OpenPopup("Confirm Delete Component");
		DrawWarningDeleteCompPopup(deleteComp != nullptr ? componentTypeNames.at(deleteComp->mCompType) : "");

		if (shouldDeleteEntity)
			ImGui::OpenPopup("Confirm Delete Entity");
		DrawWarningDeleteEntityPopup();

		if (shouldDisplayAddComp)
			ImGui::OpenPopup("AddCompPopup");

		if (ImGui::BeginPopup("AddCompPopup")) {
			for (const auto& pair : componentTypeNames) {
				if (ImGui::Selectable(pair.second)) {
					IComponent* comp = nullptr;
					try {
						comp = COMPONENT_MANAGER->mComponentCreatorsMap.at(pair.first)->Create();
						comp->mOwner = selectedEntity;
						COMPONENT_MANAGER->AddComponent(comp);
					}
					catch (...) {
						if (comp != nullptr)
							delete comp;
						shouldDisplayWarning = true;
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	void DrawGameScene() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Game Scene", NULL);
		ImVec2 buttonSize(40.0f, 30.0f);

		// Draw Play Button
		ImGui::SetCursorPos({ (ImGui::GetWindowWidth() - buttonSize.x) * 0.5f, buttonSize.y - 2.0f }); // sets image position
		if (ImGui::Button("Play", buttonSize))
			INPUT_SYSTEM->UnpauseGame();

		// Draw game scene's image
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (FUN_IMGUI_SYSTEM->mSceneViewportSize.x != viewportSize.x || FUN_IMGUI_SYSTEM->mSceneViewportSize.y != viewportSize.y) {
			GRAPHICS_SYSTEM->mFramebuffer.PreResize(static_cast<int>(viewportSize.x), static_cast<int>(viewportSize.y));
			FUN_IMGUI_SYSTEM->mSceneViewportSize = { viewportSize.x, viewportSize.y };
		}
		ImGui::Image(TextureUtils::GetImGuiTexId(GRAPHICS_SYSTEM->mFramebuffer.mTextureId), viewportSize, { 0, 1 }, { 1, 0 }); // texture is from frame buffer, see GraphicSystem constructor on how the frame buffer is created

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void DrawLoadLevelPopup() {
		if (ImGui::BeginPopupModal("Load Level", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove)) {
			ImGui::Text("Select the level to load");

			if (ImGui::BeginListBox("##Level List", { -FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing() })) {
				for (int i = 0; i < LEVEL_MANAGER->mLevels.size(); i++) {
					if (ImGui::Selectable(LEVEL_MANAGER->mLevels[i].c_str(), selectedLevel == i))
						selectedLevel = i;

					if (selectedLevel == i)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			if (ImGui::Button("Confirm Load")) {
				LEVEL_MANAGER->mShouldReloadLevel = true;
				LEVEL_MANAGER->mCurrentLevel = LEVEL_MANAGER->mLevels[selectedLevel];
				selectedLevel = -1;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				selectedLevel = -1;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawWarningPopup(const char* id, const char* message) {
		if (ImGui::BeginPopupModal(id, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove)) {
			ImGui::Text(message);

			if (ImGui::Button("Ok", {ImGui::GetContentRegionAvailWidth(), 0})) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawWarningDeleteCompPopup(const char* compType) {
		if (ImGui::BeginPopupModal("Confirm Delete Component", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove)) {
			ImGui::Text((std::string("Are you sure you want to delete component ") + compType + "?").c_str());

			if (ImGui::Button("Confirm Delete")) {
				if (deleteComp != nullptr) {
					COMPONENT_MANAGER->RemoveComponent(deleteComp);
					deleteComp = nullptr;
				}
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();

			if (ImGui::Button("Cancel")) {
				if (deleteComp != nullptr) {
					deleteComp = nullptr;
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawWarningDeleteEntityPopup() {
		if (ImGui::BeginPopupModal("Confirm Delete Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove)) {
			ImGui::Text((std::string("Are you sure you want to delete entity #") + std::to_string(selectedEntity) + "?").c_str());

			if (ImGui::Button("Confirm", { ImGui::GetContentRegionAvailWidth()*0.5f, 0 })) {
				ENTITY_MANAGER->MarkEntityDead(selectedEntity);
				selectedEntity = -1;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", { ImGui::GetContentRegionAvailWidth(), 0 })) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}