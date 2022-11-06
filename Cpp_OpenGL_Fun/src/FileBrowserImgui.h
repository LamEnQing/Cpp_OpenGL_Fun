#pragma once
#include "pch.h"
#include <filesystem>

namespace OpenGLFun {
	class FileBrowserImgui {
	public:
		FileBrowserImgui(std::string const& imguiId, std::string pathToOpen, std::vector<std::string> const& fileExtensions);
		~FileBrowserImgui();

		void OpenPopup();
		void Draw();
	private:
		bool _openPopup = false;

		std::string _popupId;
		std::filesystem::path _relativePath; // the initial path set by pathToOpen
		std::filesystem::path _currentPath;
		std::vector<std::string> _fileExtensions;
		std::string _selectedFilename;
	};
}