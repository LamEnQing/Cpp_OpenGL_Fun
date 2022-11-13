#pragma once
#include "pch.h"
#include "Configuration.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "System.h"

#include <mono/jit/jit.h>

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

			MonoDomain* GetMonoDomain();
			void GameLoop(void);
		private:
			std::vector<std::unique_ptr<ISystem>> _systems;
			float _lastTime;

			MonoDomain* _monoDomain;
	};
	extern Engine* ENGINE;
}