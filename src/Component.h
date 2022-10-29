#pragma once
#include "pch.h"
#include "EntityManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace OpenGLFun {
	enum class ComponentType {
		Camera,

		Button,
		Color,
		Model,
		Sprite,
		Transform
	};

	class IComponent {
	public:
		EntityId mOwner;
		ComponentType mCompType;

		IComponent() : mOwner(-1), mCompType(ComponentType::Camera) {}
		virtual ~IComponent() {};
		virtual void Deserialize(rapidjson::Value const& jsonObj) = 0;
		virtual void DrawImGuiComponent() = 0;
	};
}