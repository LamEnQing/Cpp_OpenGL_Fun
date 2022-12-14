#include "Sprite.h"

#include "FunImGuiSystem.h"
#include "ResourceManager.h"

namespace OpenGLFun {

	Sprite::Sprite() : IComponent(), mTextureFilepath("no_texture.png"), mUVPosition{ 0, 0 }, mUVDimensions{ 0, 0 } {
		mCompType = ComponentType::Sprite;
	}
	Sprite::~Sprite() {}

	void Sprite::Deserialize(rapidjson::Value const& jsonObj) {
		if (!jsonObj.HasMember("texture") || !jsonObj["texture"].IsString())
			throw SimpleException("Component of type Sprite must have key 'texture' with a string value");

		mTextureFilepath = jsonObj["texture"].GetString();

		if (jsonObj.HasMember("uv_pos")) {
			if (!jsonObj["uv_pos"].IsArray() || jsonObj["uv_pos"].Size() < 2)
				throw SimpleException("Component of type Sprite must have key 'uv_pos' with an array of size 2");

			const rapidjson::Value& posArr = jsonObj["uv_pos"];
			for (int i = 0; i < 2; i++) {
				if (!posArr[i].IsInt())
					throw JsonReadException("component of type Sprite", std::string("uv_pos[") + std::to_string(i) + "]", "integer");

				mUVPosition[i] = posArr[i].GetInt();
			}
		}

		if (jsonObj.HasMember("uv_dimensions")) {
			if (!jsonObj["uv_dimensions"].IsArray() || jsonObj["uv_dimensions"].Size() < 2)
				throw SimpleException("Component of type Sprite must have key 'uv_dimensions' with an array of size 2");

			const rapidjson::Value& dimArr = jsonObj["uv_dimensions"];
			for (int i = 0; i < 2; i++) {
				if (!dimArr[i].IsInt())
					throw JsonReadException("component of type Sprite", std::string("uv_dimensions[") + std::to_string(i) + "]", "integer");

				mUVDimensions[i] = dimArr[i].GetInt();
			}
		}
	}

	bool Sprite::DrawImGuiComponent() {
		bool canClose = true;
		bool openAddTexture = false;
		if (!ImGui::CollapsingHeader("Sprite", &canClose)) return !canClose;

		Texture* texturePtr = RESOURCE_MANAGER->GetTexture(mTextureFilepath);
		static int uvPosX = 0; uvPosX = mUVPosition[0];
		static int uvPosY = 0; uvPosY = mUVPosition[1];
		static int uvDimX = 0; uvDimX = mUVDimensions[0];
		static int uvDimY = 0; uvDimY = mUVDimensions[1];

		std::vector<std::string> texturesList;
		for (auto const& pair : RESOURCE_MANAGER->mTexturesDataMap)
			texturesList.push_back(pair.first);

		ImGui::Text("Texture");
		ImGui::SameLine();

		auto findResult = std::find(texturesList.begin(), texturesList.end(), mTextureFilepath);

		if (findResult != texturesList.end())
			_selectedTexture = static_cast<int>(findResult - texturesList.begin());

		if (ImGui::BeginCombo("##texture list", texturesList[_selectedTexture].c_str())) {
			for (int i = 0; i < texturesList.size(); i++) {
				std::string& texture = texturesList[i];

				if (ImGui::Selectable(texture.c_str(), _selectedTexture == i)) {
					_selectedTexture = i;
					mTextureFilepath = texture;
					texturePtr = RESOURCE_MANAGER->GetTexture(mTextureFilepath);

					uvDimX = mUVDimensions[0] = texturePtr->imgWidth;
					uvDimY = mUVDimensions[1] = texturePtr->imgHeight;
				}
				else if (mTextureFilepath == texture)
					_selectedTexture = i;
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		if (ImGui::Button("Add Texture")) {
			FUN_IMGUI_SYSTEM->mTextureLoadFileBrowser.OpenPopup();
		}

		ImGui::PushItemWidth(30);
		ImGui::Text("UV Position"); ImGui::SameLine();
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::InputInt("##uv pos x", &uvPosX, -1); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::InputInt("##uv pos y", &uvPosY, -1);

		if (uvPosX < 0)
			uvPosX = 0;
		else if (uvPosX > texturePtr->imgWidth)
			uvPosX = texturePtr->imgWidth;
		mUVPosition[0] = uvPosX;

		if (uvPosY < 0)
			uvPosY = 0;
		else if (uvPosY > texturePtr->imgHeight)
			uvPosY = texturePtr->imgHeight;
		mUVPosition[1] = uvPosY;

		ImGui::Text("UV Dimensions"); ImGui::SameLine();
		ImGui::Text("X"); ImGui::SameLine();
		ImGui::InputInt("##uv dim x", &uvDimX, -1); ImGui::SameLine();
		ImGui::Text("Y"); ImGui::SameLine();
		ImGui::InputInt("##uv dim y", &uvDimY, -1);

		if (uvDimX < 0)
			uvDimX = 0;
		else if (uvDimX > texturePtr->imgWidth)
			uvDimX = texturePtr->imgWidth;
		mUVDimensions[0] = uvDimX;

		if (uvDimY < 0)
			uvDimY = 0;
		else if (uvDimY > texturePtr->imgHeight)
			uvDimY = texturePtr->imgHeight;
		mUVDimensions[1] = uvDimY;

		ImGui::PopItemWidth();

		return !canClose;
	}
}