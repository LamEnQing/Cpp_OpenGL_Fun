#pragma once
#include "pch.h"
#include "EntityManager.h"

namespace OpenGLFun {
	enum class ComponentType {
		Camera,

		Button,
		Color,
		Model,
		Sprite,
		Transform,
	};

	class IComponent {
	public:
		EntityId mOwner;
		ComponentType mCompType;

		IComponent() : mOwner(-1), mCompType(ComponentType::Camera) {}
		virtual ~IComponent() {};
		virtual void Deserialize(rapidjson::Value const& jsonObj) = 0;
	};
}