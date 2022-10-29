#pragma once

#include "System.h"

namespace OpenGLFun {
	class FunImGuiSystem : public ISystem {
	public:
		bool mShowEditor;

		FunImGuiSystem();
		~FunImGuiSystem() override;
		void Update(float const&) override;

		// Resets variables in the system
		void Reset();
	private:
		bool _showLevelSelect;
	};

	extern FunImGuiSystem* FUN_IMGUI_SYSTEM;
}