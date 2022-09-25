#pragma once
#include "Component.h"
#include "EntityManager.h"

namespace OpenGLFun {
	class Position : public IComponent {
	public:
		glm::vec3 mPosition;

		Position();
		Position(EntityId const& owner);
		~Position() override;
		void Deserialize(rapidjson::Value const&) override;
	};
}