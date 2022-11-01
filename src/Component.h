#pragma once
#include "pch.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_stdlib.h>

namespace OpenGLFun {
	class IComponent {
	public:
		EntityId mOwner;
		ComponentType mCompType;

		IComponent() : mOwner(-1), mCompType(ComponentType::Camera) {}
		virtual ~IComponent() {};
		virtual void Deserialize(rapidjson::Value const& jsonObj) = 0;
		// returns true if wants to close, otherwise false
		virtual bool DrawImGuiComponent() = 0;
	};
}