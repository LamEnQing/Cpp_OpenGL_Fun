#include "FileBrowserImgui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "ResourceManager.h"
#include "StringUtils.h"
#include "TextureUtils.h"

namespace OpenGLFun {
	FileBrowserImgui::FileBrowserImgui(std::string const& imguiId, std::string pathToOpen, std::vector<std::string> const& fileExtensions)
		: _popupId(imguiId), _relativePath(pathToOpen), _currentPath(pathToOpen), _fileExtensions(fileExtensions) {
	}

	FileBrowserImgui::~FileBrowserImgui() {}

	void FileBrowserImgui::Draw() {
		if (_openPopup) {
			_selectedFilename = "";
			ImGui::OpenPopup(_popupId.c_str());
			_openPopup = false;
		}

		bool shouldClose = true;
		bool canAddTexture = true;
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
		if (ImGui::BeginPopupModal(_popupId.c_str(), &shouldClose, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar)) {
			if (_currentPath != _relativePath) {
				if (ImGui::ImageButton(TextureUtils::GetImGuiTexId("icons\\back_button.png"), { 15, 15 }, { 0, 1 }, { 1, 0 })) {
					_currentPath = _currentPath.parent_path();
					_selectedFilename = "";
				}
				ImGui::SameLine();
			}

			ImGui::Text(_currentPath.string().c_str());
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
			ImGui::Separator();

			// ==== Draw Filename list ====
			static ImVec2 thumbnailSize{ 20, 20 };
			static float iconPos = 10, iconDescPos = iconPos + thumbnailSize.x + 6;

			float color = 0.2;
			ImGui::PushStyleColor(ImGuiCol_FrameBg, { color, color, color, 1.0f });
			if (ImGui::BeginListBox((std::string("##") + _popupId + "_list").c_str(), {-FLT_MIN, 8 * ImGui::GetTextLineHeightWithSpacing()})) {
				for (auto const& dir_entry : std::filesystem::directory_iterator(_currentPath)) {
					if (!dir_entry.is_directory()) continue;
					std::string name = dir_entry.path().filename().string();

					ImGui::Selectable((std::string("##") + name).c_str());
					if (ImGui::IsItemHovered()) {
						ImGui::SetTooltip("Double click me to enter!");
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							_currentPath = dir_entry.path();
							_selectedFilename = "";
						}
					}
					ImGui::SameLine();
					ImGui::SetCursorPosX(iconPos);
					ImGui::Image(TextureUtils::GetImGuiTexId("icons\\folder.png"), thumbnailSize); ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
					ImGui::Text(name.c_str());
				}

				for (auto const& dir_entry : std::filesystem::directory_iterator(_currentPath)) {
					if (dir_entry.is_directory()) continue;
					std::string name = dir_entry.path().filename().string();

					bool shouldDisplay = false;
					for (std::string const& fileExt : _fileExtensions)
						if (StringUtils::ToLower(dir_entry.path().extension().string()) == StringUtils::ToLower(fileExt)) {
							shouldDisplay = true;
						}
					if (!shouldDisplay) continue;

					if (ImGui::Selectable((std::string("##") + name).c_str()))
						//_selectedFilename = std::filesystem::relative(dir_entry.path(), _relativePath).string();
						_selectedFilename = name;
					ImGui::SameLine();
					ImGui::SetCursorPosX(iconPos);
					ImGui::Image(TextureUtils::GetImGuiTexId("icons\\file.png"), thumbnailSize); ImGui::SameLine();
					ImGui::Text(name.c_str());
				}

				ImGui::EndListBox();
			}
			ImGui::PopStyleColor();
			// ==== End File list ====

			ImGui::Separator();
			float oldCursorY = ImGui::GetCursorPosY() + 4;
			ImGui::SetCursorPosY(oldCursorY + 3);
			ImGui::Text("File Name:"); ImGui::SameLine();
			ImGui::SetCursorPosY(oldCursorY);
			ImGui::SetNextItemWidth(200.0f);
			ImGui::InputText((std::string("##") + _popupId + "_filename_input").c_str(), &_selectedFilename);

			ImGui::SameLine();
			const char* previewText = _selectedExtension == 0 ? "All files" : _fileExtensions[_selectedExtension - 1].c_str();
			ImGui::SetNextItemWidth(100);
			if (ImGui::BeginCombo("##select file extensions", previewText)) {
				if (ImGui::Selectable("All files", _selectedExtension == 0))
					_selectedExtension = 0;
				for (int i = 0; i < _fileExtensions.size(); i++) {
					if (ImGui::Selectable(_fileExtensions[i].c_str(), (_selectedExtension - 1) == i))
						_selectedExtension = i + 1;
				}
				ImGui::EndCombo();
			}

			ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 83);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			static std::filesystem::path fileRelativePath;
			ImGui::BeginDisabled(_selectedFilename.empty());
			if (ImGui::Button("Add Texture") && !_selectedFilename.empty()) {
				try {
					fileRelativePath = std::filesystem::relative(_currentPath / _selectedFilename, _relativePath);
					/*std::cout << "curr path:" << _currentPath.string() << std::endl;
					std::cout << "relative path:" << _relativePath.string() << std::endl;
					std::cout << "Adding texture:" << fileRelativePath.string() << std::endl;
					std::cout << "test path:" << _currentPath /_selectedFilename  << std::endl;*/
					RESOURCE_MANAGER->LoadTexture(fileRelativePath.string());

					ImGui::CloseCurrentPopup();
				}
				catch (std::exception& e) {
					canAddTexture = false;
				}
			}
			ImGui::EndDisabled();

			if (!canAddTexture)
				ImGui::OpenPopup("Add Texture Error");

			shouldClose = true;
			if (ImGui::BeginPopupModal("Add Texture Error", &shouldClose, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text((std::string("Could not open texture to ") + fileRelativePath.string() + "!").c_str());
				if (ImGui::Button("Ok!", { ImGui::GetContentRegionAvailWidth(), 0 }))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
	}

	void FileBrowserImgui::OpenPopup() {
		_openPopup = true;
	}
}