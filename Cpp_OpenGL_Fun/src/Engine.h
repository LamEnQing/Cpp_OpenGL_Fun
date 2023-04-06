#pragma once
#include "pch.h"
#include "Configuration.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "System.h"

namespace OpenGLFun {
	class Engine {
		public:
			Configuration mConfig;
			EntityId mPlayerId;
			bool mInDebugMode;
			bool mIsPaused;
			bool mShouldMouseBeLocked;

			Engine();
			~Engine();

			void GameLoop(void);

			std::vector<std::unique_ptr<ISystem>>& GetSystems();
		private:
			std::vector<std::unique_ptr<ISystem>> _systems;
			float _lastTime;
	};
	extern Engine* ENGINE;
}