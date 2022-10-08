#pragma once
#include "pch.h"
#include "System.h"

namespace OpenGLFun {
	class AnimationSystem : public ISystem {
	public:
		AnimationSystem();
		~AnimationSystem();
		virtual void Update(float const&) override;
	};
}