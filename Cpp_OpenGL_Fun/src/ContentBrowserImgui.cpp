#include "ContentBrowserImgui.h"

#include <imgui/imgui.h>
#include <iostream>

#include "GraphicSystem.h"
#include "ResourceManager.h"
#include "StringUtils.h"
#include "TextureUtils.h"

namespace OpenGLFun {

	static const std::filesystem::path ASSETS_PATH = "assets";

	ContentBrowserImgui::ContentBrowserImgui() : _currPath(ASSETS_PATH), _shouldCreatePreviewIcons{ true } {}

	ContentBrowserImgui::~ContentBrowserImgui() {
		for (auto& pair : previewIconTextures) {
			GRAPHICS_SYSTEM->DeleteGLTexture(pair.second.get());
		}
		previewIconTextures.clear();
	}

	void ContentBrowserImgui::Draw() {
		static std::set<std::string> imageExtensions = {
			".jpg", ".png"
		};

		ImGui::Begin("Content Browser");

		if (_currPath != ASSETS_PATH)
		{
			if (ImGui::ImageButton(TextureUtils::GetImGuiTexId("icons\\back_button.png"), {15, 15}, {0, 1}, {1, 0})) {
				_currPath = _currPath.parent_path();

				_shouldCreatePreviewIcons = true;
			}
			ImGui::SameLine();
		}

		float padding = 10.0f;
		float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;
		ImVec2 minFramePadding{ 4.0f, 3.0f };

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columns = static_cast<int>(panelWidth / cellSize);
		if (columns < 1)
			columns = 1;

		ImGui::Text(_currPath.string().c_str());
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
		ImGui::Separator();
		
		ImGui::BeginChild("##content browser icon lists");
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
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						_currPath = dir_entry.path();

						_shouldCreatePreviewIcons = true;
					}
				}
			}

			for (auto const& dir_entry : std::filesystem::directory_iterator(_currPath)) {
				if (dir_entry.is_directory()) continue;
				ImGui::TableNextColumn();

				std::string fileExt = StringUtils::ToLower(dir_entry.path().extension().string());
				std::string filename = dir_entry.path().filename().string();

				if (_shouldCreatePreviewIcons && imageExtensions.find(fileExt) != imageExtensions.end() && previewIconTextures.find(filename) == previewIconTextures.end()) {
					Texture* texture = GRAPHICS_SYSTEM->CreateGLTexture(_currPath.string() + "\\" + filename);

					previewIconTextures.insert({ filename, std::unique_ptr<Texture>(texture) });
				}

				ImTextureID buttonTexId = TextureUtils::GetImGuiTexId("icons\\file.png");
				ImVec2 imgBtnSize = { thumbnailSize, thumbnailSize };

				if (previewIconTextures.find(filename) != previewIconTextures.end()) {
					Texture* tex = previewIconTextures.at(filename).get();
					buttonTexId = TextureUtils::GetImGuiTexId(tex->mGLTextureId);

					// og_w/og_h = resize_w/resize_h. So you want to get a resize dimension
					float imgRes = tex->imgWidth / static_cast<float>(tex->imgHeight);
					if (tex->imgWidth > tex->imgHeight) {
						imgBtnSize.y = thumbnailSize / imgRes;
					}
					else if (tex->imgHeight > tex->imgWidth) {
						imgBtnSize.x = thumbnailSize * imgRes;
					}
				}

				ImGui::BeginGroup();
				ImGui::PushID(filename.c_str());
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { minFramePadding.x + (thumbnailSize - imgBtnSize.x) / 2.0f, minFramePadding.y + (thumbnailSize - imgBtnSize.y) / 2.0f });
				ImGui::ImageButton(buttonTexId, imgBtnSize, { 0, 1 }, { 1, 0 });
				ImGui::PopStyleVar();
				ImGui::Text(filename.c_str());
				ImGui::PopID();

				ImGui::EndGroup();
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip(filename.c_str());
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						std::string absPath = std::filesystem::absolute(dir_entry.path()).string();
						/*LPCWSTR sw = std::wstring(absPath.begin(), absPath.end()).c_str();
						HINSTANCE status = ShellExecute(0, 0, sw, 0, 0, SW_SHOW); // from: https://stackoverflow.com/questions/9115574/how-can-you-open-a-file-with-the-program-associated-with-its-file-extension
						std::cout << "ShellExecute stats:" << status << std::endl;*/
					}
				}
			}

			if (_shouldCreatePreviewIcons)
				_shouldCreatePreviewIcons = false;

			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();

		ImGui::End();
	}
}