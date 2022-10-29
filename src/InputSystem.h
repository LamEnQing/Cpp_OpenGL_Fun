#pragma once
#include "pch.h"
#include "System.h"

namespace OpenGLFun {
	class InputSystem : public ISystem {
	public:
		glm::vec2 mMousePosOld;
		glm::vec2 mMousePos;
		glm::vec2 mMousePosUnlocked; // the mouse pos before the mouse was locked

		float mMouseSensitivity;

		InputSystem();
		~InputSystem() override;
		virtual void Update(float const&) override;

		int IsKeyHeld(int key);
		bool IsKeyTriggered(int key);
		bool IsMouseButtonPressed(int mouseButton);

		void LockMouse();
		void UnlockMouse();
		void PauseGame();
		void UnpauseGame();
	private:
		std::vector<int> _availableKeys;
		std::map<int, bool> _keyTriggerStateMap;
	};

	extern InputSystem* INPUT_SYSTEM;
}