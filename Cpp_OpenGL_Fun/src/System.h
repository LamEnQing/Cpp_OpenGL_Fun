#pragma once
namespace OpenGLFun {
	class ISystem {
	public:
		ISystem() {}
		virtual ~ISystem() {}
		virtual void Update(float const&) = 0;
	};
}