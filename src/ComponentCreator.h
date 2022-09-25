#pragma once
#include "Component.h"

namespace OpenGLFun {
	class IComponentCreator {
	public:
		IComponentCreator() {}
		virtual ~IComponentCreator() {}
		virtual IComponent* Create() = 0;
	};

	template<typename T>
	class ComponentCreator : public IComponentCreator {
	public:
		ComponentCreator() {}
		~ComponentCreator() {}
		IComponent* Create() {
			return new T();
		}
	};
}