#pragma once
#include "pch.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "System.h"

namespace OpenGLFun {
	class Engine {
		public:
			EntityId mPlayerId;
			bool mInDebugMode;

			Engine();
			~Engine();

			void GameLoop(void);
		private:
			std::vector<std::unique_ptr<ISystem>> _systems;
			float _lastTime;
	};
}