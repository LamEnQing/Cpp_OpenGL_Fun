#pragma once
#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "Texture.h"

namespace OpenGLFun {
	class ContentBrowserImgui {
	public:
		ContentBrowserImgui();
		~ContentBrowserImgui();

		void Draw();
	private:
		std::filesystem::path _currPath;
		std::map<std::string, std::unique_ptr<Texture>> previewIconTextures;
		bool _shouldCreatePreviewIcons;
	};
}