#include "ContentBrowserImgui.h"

#include <imgui/imgui.h>
#include <iostream>

#include "ResourceManager.h"
#include "TextureUtils.h"

#include <Windows.h>
#include <shellapi.h>

namespace OpenGLFun {

	static const std::filesystem::path ASSETS_PATH = "assets";

	ContentBrowserImgui::ContentBrowserImgui() : _currPath(ASSETS_PATH) {}

	ContentBrowserImgui::~ContentBrowserImgui() {}

	void ContentBrowserImgui::Draw() {
		ImGui::Begin("Content Browser");

		if (_currPath != ASSETS_PATH)
		{
			if (ImGui::ImageButton(TextureUtils::GetImGuiTexId("icons\\back_button.png"), {15, 15}, {0, 1}, {1, 0})) {
				_currPath = _currPath.parent_path();
			}
			ImGui::SameLine();
		}

		float padding = 10.0f;
		float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvailWidth();
		int columns = static_cast<int>(panelWidth / cellSize);
		if (columns < 1)
			columns = 1;

		ImGui::Text(_currPath.string().c_str());
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
		ImGui::Separator();
		
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { padding, 5.0f });
		if (ImGui::BeginTable("contentBrowserTable", columns)) {
			for (auto const& dir_entry : std::filesystem::directory_iterator(_currPath)) {
				if (!dir_entry.is_directory()) continue;

				ImGui::PushItemWidth(cellSize); // Small
				ImGui::TableNextColumn();

				std::string folderName = dir_entry.path().filename().string();

				ImGui::BeginGroup();
				ImGui::PushID(folderName.c_str());
				ImGui::ImageButton(TextureUtils::GetImGuiTexId("icons\\folder.png"), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				ImGui::Text(folderName.c_str());
				ImGui::PopID();
				ImGui::EndGroup();
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip(folderName.c_str());
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						_currPath = dir_entry.path();
				}
			}
			for (auto const& dir_entry : std::filesystem::directory_iterator(_currPath)) {
				if (dir_entry.is_directory()) continue;
				ImGui::TableNextColumn();

				std::string filename = dir_entry.path().filename().string();

				ImGui::BeginGroup();
				ImGui::PushID(filename.c_str());
				ImGui::ImageButton(TextureUtils::GetImGuiTexId("icons\\file.png"), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				//ImGui::Render6
				ImGui::Text(filename.c_str());
				ImGui::PopID();

				ImGui::EndGroup();
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip(filename.c_str());
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						std::string absPath = std::filesystem::absolute(dir_entry.path()).string();
						LPCWSTR sw = std::wstring(absPath.begin(), absPath.end()).c_str();
						HINSTANCE status = ShellExecute(0, 0, sw, 0, 0, SW_SHOW); // from: https://stackoverflow.com/questions/9115574/how-can-you-open-a-file-with-the-program-associated-with-its-file-extension
						std::cout << "ShellExecute stats:" << status << std::endl;
					}
				}
			}
			ImGui::EndTable();
		}
		ImGui::PopStyleVar();

		ImGui::End();
	}
}