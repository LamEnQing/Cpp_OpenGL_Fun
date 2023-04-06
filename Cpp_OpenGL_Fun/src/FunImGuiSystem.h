#pragma once

#include "System.h"

#include "ContentBrowserImgui.h"
#include "FileBrowserImgui.h"
#include "OrthoCamera2D.h"
#include "Vec2.h"

namespace OpenGLFun {
	class FunImGuiSystem : public ISystem {
	public:
		Vec2<float> mSceneViewportSize;
		FileBrowserImgui mTextureLoadFileBrowser;
		OrthoCamera mCamera;

		FunImGuiSystem();
		~FunImGuiSystem() override;
		void Update(float const&) override;

		// Resets variables in the system
		void Reset();

		bool ShowEditor() const;
		void ShowEditor(bool value);
	private:
		bool _showEditor;

		ContentBrowserImgui _contentBrowser;
	};

	extern FunImGuiSystem* FUN_IMGUI_SYSTEM;
}