#pragma once

#include "pch.h"

#include "System.h"

namespace OpenGLFun {
	class LogicSystem : public ISystem {
		public:
			LogicSystem();
			~LogicSystem() override;
			virtual void Update(float const& deltaTime) override;
	private:
	};

	extern LogicSystem* LOGIC_SYSTEM;
}