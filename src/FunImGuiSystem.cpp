#include "FunImGuiSystem.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "pch.h"
#include "Engine.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "WindowSystem.h"
#include "Sprite.h"

namespace OpenGLFun {
	FunImGuiSystem* FUN_IMGUI_SYSTEM = nullptr;
	int selectedEntity = -1;
	int selectedAddComponent = 0;
	int selectedLevel = -1;

	IComponent* deleteComp = nullptr; // the component to be deleted

	void DrawEntityList();
	void DrawEntityProperty();
	void DrawGameScene();
	void DrawLoadLevelPopup();
	void DrawWarningPopup(const char* id, const char* message);
	void DrawWarningDeleteCompPopup(const char* compType);

	FunImGuiSystem::FunImGuiSystem() : mShowEditor{ false }, _showLevelSelect{ false } {
		if (FUN_IMGUI_SYSTEM != nullptr)
			throw SimpleException("FunImGuiSystem has already been created!");

		FUN_IMGUI_SYSTEM = this;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

	void FunImGuiSystem::Update(float const& deltaTime) {
		if (mShowEditor) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuiIO& io = ImGui::GetIO();

			bool shouldShowLevelSelect = false;
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					ImGui::MenuItem("Load Level", NULL, &shouldShowLevelSelect);

					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode, NULL);

			if (shouldShowLevelSelect) {
				std::cout << "Hello pop up should be opening\n";
				ImGui::OpenPopup("Load Level");
			}

			DrawEntityList();
			DrawEntityProperty();
			DrawGameScene();

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
		ImGui::End();
	}

	void DrawEntityProperty() {
		static std::map<ComponentType, const char*> componentTypeNames{
			{ ComponentType::Button, "Button" },
			{ ComponentType::Camera, "Camera" },
			{ ComponentType::Color, "Color" },
			{ ComponentType::Model, "Model" },
			{ ComponentType::Sprite, "Sprite" },
			{ ComponentType::Transform, "Transform" }
		};

		bool shouldDisplayWarning = false;

		ImGui::Begin("Entity Property", NULL, ImGuiWindowFlags_HorizontalScrollbar);
		if (selectedEntity == -1) {
			ImGui::Text("Select an entity!");
		}
		else {
			static const char* items[]{ "Button", "Color", "Model", "Sprite", "Transform" };

			ImGui::SetNextItemWidth(ImGui::CalcTextSize(items[selectedAddComponent], NULL, true).x + 30.0f);
			if (ImGui::BeginCombo("##Add Component Combo", items[selectedAddComponent])) {
				for (size_t i = 0; i < IM_ARRAYSIZE(items); i++) {
					if (ImGui::Selectable(items[i], selectedAddComponent == i)) {
						selectedAddComponent = i;
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (ImGui::Button("Add Component")) {
				auto findResult = std::find_if(componentTypeNames.begin(), componentTypeNames.end(), [&](const std::pair<ComponentType, const char*>& pair) {
					return pair.second == items[selectedAddComponent];
				});

				IComponent* comp = nullptr;
				try {
					IComponent* comp = COMPONENT_MANAGER->mComponentCreatorsMap.at(findResult->first)->Create();
					comp->mOwner = selectedEntity;
					if (comp->mCompType == ComponentType::Sprite) {
						dynamic_cast<Sprite*>(comp)->mTextureFilepath = "no_texture.png";
					}
					COMPONENT_MANAGER->AddComponent(comp);
				}
				catch (...) {
					if (comp != nullptr)
						delete comp;
					shouldDisplayWarning = true;
				}
			}
			
			for (IComponent* comp : COMPONENT_MANAGER->GetEntityComponents(selectedEntity)) {
				if (componentTypeNames.find(comp->mCompType) == componentTypeNames.end()) continue;

				bool canClose = true;
				if (ImGui::CollapsingHeader(componentTypeNames.at(comp->mCompType), &canClose, ImGuiTreeNodeFlags_DefaultOpen)) {
					comp->DrawImGuiComponent();
				}

				if (!canClose) {
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
	}

	void DrawGameScene() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Game Scene", NULL);
		ImVec2 buttonSize(40.0f, 30.0f);

		// Draw game scene's image
		float windowWidth = ImGui::GetWindowSize().x;
		float windowHeight = ImGui::GetWindowSize().y - buttonSize.y;
		float calcWindowWidth = (16.0f/9.0f) * windowHeight; // 16/9 = width/height. So to get width, 16 / 9 * height
		float calcWindowHeight = windowWidth / (16.0f / 9.0f); // 16/9 = width/height. So to get height, width / (16/9)

		ImVec2 imgSize = {windowWidth, windowHeight};
		if (calcWindowWidth <= windowWidth) // if calculated width is less than actual width, using the width
			imgSize.x = calcWindowWidth;
		else 
			imgSize.y = calcWindowHeight;

		ImGui::SetCursorPos({ (windowWidth - imgSize.x) * 0.5f , 0.0f }); // sets image position
		ImGui::Image((ImTextureID)GRAPHICS_SYSTEM->mFrameBufferTex, imgSize, { 0, 1 }, { 1, 0 }); // texture is from frame buffer, see GraphicSystem constructor on how the frame buffer is created

		// Draw Play Button
		ImGui::SetCursorPos({ (windowWidth - buttonSize.x)*0.5f , imgSize.y }); // sets image position
		bool shouldEndDisable = false;
		if (!ENGINE->mIsPaused) {
			ImGui::BeginDisabled();
			shouldEndDisable = true;
		}
		if (ImGui::Button("Play", buttonSize))
			INPUT_SYSTEM->UnpauseGame();
		if (!ENGINE->mIsPaused && shouldEndDisable)
			ImGui::EndDisabled();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void DrawLoadLevelPopup() {
		if (ImGui::BeginPopupModal("Load Level", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
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
		if (ImGui::BeginPopupModal(id, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text(message);
			const ImVec2 label_size = ImGui::CalcTextSize("Ok", NULL, true);

			if (ImGui::Button("Ok", {ImGui::GetContentRegionAvailWidth(), label_size.y + 4})) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawWarningDeleteCompPopup(const char* compType) {
		if (ImGui::BeginPopupModal("Confirm Delete Component", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text((std::string("Are you sure you want to delete component ") + compType + "?").c_str());
			const ImVec2 label_size = ImGui::CalcTextSize("Ok", NULL, true);

			if (ImGui::Button("Ok", { ImGui::GetContentRegionAvailWidth(), label_size.y + 4 })) {
				if (deleteComp != nullptr) {
					COMPONENT_MANAGER->RemoveComponent(deleteComp);
					deleteComp = nullptr;
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}