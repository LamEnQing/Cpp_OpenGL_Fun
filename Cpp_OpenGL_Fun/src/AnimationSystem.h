#pragma once
#include "pch.h"
#include "System.h"

namespace OpenGLFun {
	class AnimationSystem : public ISystem {
	public:
		AnimationSystem();
		~AnimationSystem() override;
		virtual void Update(float const&) override;
	};
}