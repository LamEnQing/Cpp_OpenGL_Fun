#include "LogicSystem.h"

#include "Engine.h"

namespace OpenGLFun {
	LogicSystem* LOGIC_SYSTEM = nullptr;

	LogicSystem::LogicSystem() {
		if (LOGIC_SYSTEM)
			throw SimpleException("Logic system already created!");

		LOGIC_SYSTEM = this;
	}

	LogicSystem::~LogicSystem() {}

	void LogicSystem::Update(float const& /*deltaTime*/) {

	}
}