#pragma once

#include "pch.h"

#include <mono/metadata/assembly.h>

#include "System.h"

namespace OpenGLFun {
	class LogicSystem : public ISystem {
		public:
			LogicSystem();
			~LogicSystem() override;
			virtual void Update(float const& deltaTime) override;

			const std::vector<std::string>& GetBehaviourList() const;
	private:
		MonoAssembly* _monoApiAssembly;
		MonoImage* _monoApiImage;

		MonoAssembly* _monoAppAssembly;
		MonoImage* _monoAppImage;

		std::vector<std::string> _behaviourList;
	};

	extern LogicSystem* LOGIC_SYSTEM;
}