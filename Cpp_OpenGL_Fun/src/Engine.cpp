#include "Engine.h"

#include "AnimationSystem.h"
#include "ComponentManager.h"
#include "FunImGuiSystem.h"
#include "GraphicSystem.h"
#include "InputSystem.h"
#include "LevelManager.h"
#include "LogicSystem.h"
#include "ResourceManager.h"
#include "WindowSystem.h"

#include "Camera.h"
#include "Transform.h"

namespace OpenGLFun {
	ComponentManager* COMPONENT_MANAGER = nullptr;
	EntityManager* ENTITY_MANAGER = nullptr;
	ResourceManager* RESOURCE_MANAGER = nullptr;
	std::unique_ptr<EntityFactory> ENTITY_FACTORY;
	std::unique_ptr<LevelManager> LEVEL_MANAGER;

	Engine::Engine(): mPlayerId(-1), mInDebugMode(false), mIsPaused(false), mShouldMouseBeLocked(false), _lastTime(0) {
		COMPONENT_MANAGER = new ComponentManager();
		ENTITY_MANAGER = new EntityManager();
		RESOURCE_MANAGER = new ResourceManager();
		ENTITY_FACTORY.reset(new EntityFactory());
		LEVEL_MANAGER.reset(new LevelManager());
	}

	Engine::~Engine() {
		std::cout << "Engine destructor";

		delete RESOURCE_MANAGER;
		delete COMPONENT_MANAGER;
		delete ENTITY_MANAGER;
	}

	void Engine::GameLoop(void) {
		mConfig.LoadConfig();

		_systems.push_back(std::unique_ptr<ISystem>(new WindowSystem()));
		_systems.push_back(std::unique_ptr<ISystem>(new InputSystem()));
		_systems.push_back(std::unique_ptr<ISystem>(new LogicSystem()));
		_systems.push_back(std::unique_ptr<ISystem>(new AnimationSystem()));
		_systems.push_back(std::unique_ptr<ISystem>(new GraphicSystem()));
		_systems.push_back(std::unique_ptr<ISystem>(new FunImGuiSystem()));

		mConfig.ReloadConfig(); // To load models

		LEVEL_MANAGER->LoadLevel(LEVEL_MANAGER->mCurrentLevel);
		if (mShouldMouseBeLocked)
			INPUT_SYSTEM->LockMouse();

		float totalFrameTime = 0.0f;
		int frameCount = 0;

		while (!glfwWindowShouldClose(WINDOW_SYSTEM->mWindow)) {
			float currentTime = static_cast<float>(glfwGetTime());
			float deltaTime = currentTime - _lastTime;
			_lastTime = static_cast<float>(glfwGetTime());

			for (std::unique_ptr<ISystem>& system : _systems)
				system->Update(deltaTime);

			glfwSwapBuffers(WINDOW_SYSTEM->mWindow);
			glfwSwapInterval(0);

			if (!GRAPHICS_SYSTEM->mFramebuffer.mHasResized) {
				GRAPHICS_SYSTEM->mFramebuffer.PostResize();
			}

			if (LEVEL_MANAGER->mShouldReloadLevel) {
				LEVEL_MANAGER->mShouldReloadLevel = false;
				LEVEL_MANAGER->ReloadLevel();

				if (FUN_IMGUI_SYSTEM->ShowEditor())
					INPUT_SYSTEM->PauseGame();
			}

			// this adds a time buffer, to ensure it runs at 60 fps
			do {
				currentTime = static_cast<float>(glfwGetTime()); // get current time and subtract that with the frame's start time
				deltaTime = currentTime - _lastTime;
			} while (deltaTime < 1.0f / 60.0f); // measure the delta time, ensure it doesn't exceed the time for a single frame, that is: 1 second / 60 frames

			//std::cout << "FPS:" << 1.0f/deltaTime << '\n';
			frameCount++;
			totalFrameTime += deltaTime;
			if (totalFrameTime >= 1.0) {
				//std::cout << "Frames:" << frameCount << '\n';
				totalFrameTime = 0;
				frameCount = 0;
			}
		}
	}

	std::vector<std::unique_ptr<ISystem>>& Engine::GetSystems() {
		return _systems;
	}
}