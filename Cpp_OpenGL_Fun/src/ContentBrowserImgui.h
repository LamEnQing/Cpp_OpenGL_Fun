#pragma once
#include <filesystem>

namespace OpenGLFun {
	class ContentBrowserImgui {
	public:
		ContentBrowserImgui();
		~ContentBrowserImgui();

		void Draw();
	private:
		std::filesystem::path _currPath;
	};
}