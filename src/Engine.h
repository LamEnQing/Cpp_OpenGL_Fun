#pragma once
#include "pch.h"
#include "EntityFactory.h"
#include "EntityManager.h"
#include "System.h"

namespace OpenGLFun {
	class Engine {
		public:
			EntityId mPlayerId;
			Engine();
			~Engine();

			void GameLoop(void);
		private:
			std::vector<std::unique_ptr<ISystem>> _systems;
			float _lastTime;
	};
}