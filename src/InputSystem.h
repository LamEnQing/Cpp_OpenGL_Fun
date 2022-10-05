#pragma once
#include "pch.h"
#include "System.h"

namespace OpenGLFun {
	class InputSystem : public ISystem {
	public:
		glm::vec2 mMousePosOld;
		glm::vec2 mMousePos;

		bool mIsMouseLocked;
		float mMouseSensitivity;

		InputSystem();
		virtual void Update(float const&) override;

		int IsKeyHeld(int key);
		bool IsKeyTriggered(int key);
		bool IsMouseButtonPressed(int mouseButton);

		void LockMouse();
	private:
		std::vector<int> _availableKeys;
		std::map<int, bool> _keyTriggerStateMap;
	};

	extern InputSystem* INPUT_SYSTEM;
}