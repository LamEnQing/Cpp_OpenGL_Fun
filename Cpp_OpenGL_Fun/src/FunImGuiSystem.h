#pragma once

#include "System.h"

#include "ContentBrowserImgui.h"
#include "FileBrowserImgui.h"
#include "Vec2.h"

namespace OpenGLFun {
	class FunImGuiSystem : public ISystem {
	public:
		bool mShowEditor;
		Vec2<float> mSceneViewportSize;
		FileBrowserImgui mTextureLoadFileBrowser;

		FunImGuiSystem();
		~FunImGuiSystem() override;
		void Update(float const&) override;

		// Resets variables in the system
		void Reset();

	private:
		ContentBrowserImgui _contentBrowser;
	};

	extern FunImGuiSystem* FUN_IMGUI_SYSTEM;
}