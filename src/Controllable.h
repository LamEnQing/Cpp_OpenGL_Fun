#pragma once
#include "Component.h"

namespace OpenGLFun {
	class Controllable : public IComponent {
	public:
		Controllable() : IComponent() {
			mCompType = ComponentType::Controllable;
			std::cout << "Controllable constructor\n";
		};

		Controllable(EntityId const& owner) : Controllable() {
			mOwner = owner;
		}
		~Controllable() {}
		void Deserialize(rapidjson::Value const&) {}
	};
}