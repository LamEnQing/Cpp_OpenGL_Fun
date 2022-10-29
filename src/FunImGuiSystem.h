#pragma once

#include "System.h"

namespace OpenGLFun {
	class FunImGuiSystem : public ISystem {
	public:
		bool mShowDemoWindow;

		FunImGuiSystem();
		~FunImGuiSystem() override;
		void Update(float const&) override;
	private:
		bool _showLevelSelect;
	};

	extern FunImGuiSystem* FUN_IMGUI_SYSTEM;
}