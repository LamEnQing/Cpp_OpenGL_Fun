#include "ComponentManager.h"
#include "Engine.h"
#include "LevelManager.h"
#include "InputSystem.h"
#include "Transform.h"
#include "WindowSystem.h"
#include "Camera.h"
#include "Vec2f.h"

namespace OpenGLFun {
	float _deltaTime = 0.0f;
	InputSystem* INPUT_SYSTEM = nullptr;
	void MousePosCallback(GLFWwindow* window, double xPosIn, double yPosIn);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	InputSystem::InputSystem() : ISystem(), mIsMouseLocked(true), mMouseSensitivity(1.0f), mMousePosOld{ 0.0f }, mMousePos{ 0.0f } {
		if (INPUT_SYSTEM != nullptr)
			throw SimpleException("InputSystem already created!");

		INPUT_SYSTEM = this;

		glfwSetCursorPosCallback(WINDOW_SYSTEM->mWindow, MousePosCallback);
		glfwSetKeyCallback(WINDOW_SYSTEM->mWindow, KeyCallback);

		_availableKeys = {
			GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H,
			GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_P,
			GLFW_KEY_Q, GLFW_KEY_R, GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X,
			GLFW_KEY_Y, GLFW_KEY_Z,
			GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_0,
			GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9,
			GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12,
			GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL
		};
	}

	void InputSystem::Update(float const& deltaTime) {
		_deltaTime = deltaTime;

		for (int& key : _availableKeys) {
			if (INPUT_SYSTEM->_keyTriggerStateMap.find(key) == INPUT_SYSTEM->_keyTriggerStateMap.end()) { // could not find, means key does not exist in the map
				if (IsKeyHeld(key))
					INPUT_SYSTEM->_keyTriggerStateMap.insert({ key, true });
			}
			else {
				if (IsKeyHeld(key))
					INPUT_SYSTEM->_keyTriggerStateMap.at(key) = false;
				else
					INPUT_SYSTEM->_keyTriggerStateMap.erase(key);
			}
		}

		if (IsKeyTriggered(GLFW_KEY_F3) == GLFW_PRESS) {
			LEVEL_MANAGER.get()->ReloadLevel();
		}

		if (IsKeyTriggered(GLFW_KEY_F2) == GLFW_PRESS) {
			ENGINE->mInDebugMode = !ENGINE->mInDebugMode;
		}

		if (ENGINE->mIsPaused || ENGINE->mPlayerId == -1 || !COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Camera) || !COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Transform)) return;

		Camera* playerCamera = COMPONENT_MANAGER->GetComponent<Camera>(ENGINE->mPlayerId, ComponentType::Camera);
		Transform* playerTransform = COMPONENT_MANAGER->GetComponent<Transform>(ENGINE->mPlayerId, ComponentType::Transform);

		float playerSpeed = 3.0f * deltaTime;
		//float rotation = 10.0f * deltaTime;
		if (IsKeyHeld(GLFW_KEY_W)) {
			playerTransform->mPositionOld = playerTransform->mPosition;
			playerTransform->mPosition += OpenGLFun::normalize(playerCamera->mLookAt) * playerSpeed;
		}
		else if (IsKeyHeld(GLFW_KEY_S)) {
			playerTransform->mPositionOld = playerTransform->mPosition;
			playerTransform->mPosition -= OpenGLFun::normalize(playerCamera->mLookAt) * playerSpeed;
		}

		if (IsKeyHeld(GLFW_KEY_A)) {
			playerTransform->mPositionOld = playerTransform->mPosition;
			playerTransform->mPosition -= OpenGLFun::normalize(OpenGLFun::cross(playerCamera->mLookAt, playerCamera->mCamUp)) * playerSpeed;
		}
		else if (IsKeyHeld(GLFW_KEY_D)) {
			playerTransform->mPositionOld = playerTransform->mPosition;
			playerTransform->mPosition += OpenGLFun::normalize(cross(playerCamera->mLookAt, playerCamera->mCamUp)) * playerSpeed;
		}

		if (IsKeyHeld(GLFW_KEY_SPACE)) {
			playerTransform->mPositionOld = playerTransform->mPosition;
			playerTransform->mPosition += playerCamera->mCamUp * playerSpeed;
		}
		else if (IsKeyHeld(GLFW_KEY_LEFT_CONTROL)) {
			playerTransform->mPositionOld = playerTransform->mPosition;
			playerTransform->mPosition -= playerCamera->mCamUp * playerSpeed;
		}

		if (playerTransform->mPositionOld != playerTransform->mPosition) {
			//printf("Pos: %.2f, %.2f, %.2f\n", playerTransform->pos.x, playerTransform->pos.y, playerTransform->pos.z);
			playerTransform->mPositionOld = playerTransform->mPosition;
		}
		if (playerTransform->mRotationOld != playerTransform->mRotation) {
			//printf("X Head Rot: %.2f\n", playerTransform->headXRot);
			playerTransform->mRotationOld = playerTransform->mRotation;
		}
	}

	int InputSystem::IsKeyHeld(int key) {
		return glfwGetKey(WINDOW_SYSTEM->mWindow, key);
	}

	bool InputSystem::IsKeyTriggered(int key) {
		if (_keyTriggerStateMap.find(key) == _keyTriggerStateMap.end())
			return false;
		return _keyTriggerStateMap.at(key);
	}

	bool InputSystem::IsMouseButtonPressed(int mouseButton) {
		return glfwGetMouseButton(WINDOW_SYSTEM->mWindow, mouseButton);
	}

	void InputSystem::LockMouse() {
		if (!ENGINE->mShouldMouseBeLocked) return;
		INPUT_SYSTEM->mIsMouseLocked = true;
		glfwSetInputMode(WINDOW_SYSTEM->mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(WINDOW_SYSTEM->mWindow, static_cast<double>(WINDOW_SYSTEM->GetWindowWidth()) / 2.0, static_cast<double>(WINDOW_SYSTEM->GetWindowHeight()) / 2.0);
		INPUT_SYSTEM->mMousePosOld = { static_cast<float>(WINDOW_SYSTEM->GetWindowWidth()) / 2.0f, static_cast<float>(WINDOW_SYSTEM->GetWindowHeight()) / 2.0f };
	}

	void MousePosCallback(GLFWwindow*, double xPosIn, double yPosIn) {
		INPUT_SYSTEM->mMousePos = { xPosIn, yPosIn };

		if (ENGINE->mIsPaused) return;

		if (ENGINE->mPlayerId != -1 && COMPONENT_MANAGER->HasComponent(ENGINE->mPlayerId, ComponentType::Camera)) {
			//printf("Mouse Input: %.2f, %.2f\n", xPosIn, yPosIn);
			//printf("Mouse Old: %.2f, %.2f\n", INPUT_SYSTEM->mMousePosOld.x, INPUT_SYSTEM->mMousePosOld.y);

			Vec2f mouseOffset = { static_cast<float>(xPosIn) - INPUT_SYSTEM->mMousePosOld.x, INPUT_SYSTEM->mMousePosOld.y - static_cast<float>(yPosIn) }; // mouse origin starts from top-left, so for y it has to be old-new to get the correct signed value
			INPUT_SYSTEM->mMousePosOld = { xPosIn, yPosIn };
			//printf("Mouse Offset: %.2f, %.2f\n", mouseOffset.x, mouseOffset.y);

			mouseOffset *= 1.0f * INPUT_SYSTEM->mMouseSensitivity * _deltaTime;

			Camera* playerCamera = COMPONENT_MANAGER->GetComponent<Camera>(ENGINE->mPlayerId, ComponentType::Camera);
			Transform* playerTransform = COMPONENT_MANAGER->GetComponent<Transform>(ENGINE->mPlayerId, ComponentType::Transform);

			/*std::cout << "Player Rotation Old:" << playerTransform->mRotationOld << '\n';
			std::cout << "Player Rotation:" << playerTransform->mRotation << '\n';*/

			playerTransform->mRotationOld.x = playerTransform->mRotation.x;
			playerTransform->mRotationOld.y = playerTransform->mRotation.y;

			playerTransform->mRotation.x += mouseOffset.x;
			playerTransform->mRotation.y += mouseOffset.y;

			if (playerTransform->mRotation.y > 89.0f)
				playerTransform->mRotation.y = 89.0f;
			else if (playerTransform->mRotation.y < -89.0f)
				playerTransform->mRotation.y = -89.0f;

			Vec3f dir(1.0f);
			dir.x = cos(glm::radians(playerTransform->mRotation.x)) * cos(glm::radians(playerTransform->mRotation.y));
			dir.y = sin(glm::radians(playerTransform->mRotation.y));
			dir.z = sin(glm::radians(playerTransform->mRotation.x)) * cos(glm::radians(playerTransform->mRotation.y));
			playerCamera->mLookAtPrev = playerCamera->mLookAt;
			playerCamera->mLookAt = OpenGLFun::normalize(dir);
		}
	}

	void KeyCallback(GLFWwindow* window, int key, int, int action, int) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				if (!ENGINE->mIsPaused) {
					INPUT_SYSTEM->mIsMouseLocked = false;
					ENGINE->mIsPaused = true;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					std::cout << "Game paused\n";
				}
				else {
					if (ENGINE->mShouldMouseBeLocked) {
						INPUT_SYSTEM->LockMouse();
					}
					ENGINE->mIsPaused = false;
					std::cout << "Game unpaused\n";
				}
			}
		}
	}
}