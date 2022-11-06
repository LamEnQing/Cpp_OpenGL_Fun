#pragma once

#include "System.h"

#include "ContentBrowserImgui.h"
#include "Vec2.h"

namespace OpenGLFun {
	class FunImGuiSystem : public ISystem {
	public:
		bool mShowEditor;
		Vec2<float> mSceneViewportSize;

		FunImGuiSystem();
		~FunImGuiSystem() override;
		void Update(float const&) override;

		// Resets variables in the system
		void Reset();
	private:
		bool _showLevelSelect;
		ContentBrowserImgui _contentBrowser;
	};

	extern FunImGuiSystem* FUN_IMGUI_SYSTEM;
}