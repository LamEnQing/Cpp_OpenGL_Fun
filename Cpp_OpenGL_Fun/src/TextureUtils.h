#pragma once
#include "pch.h"
#include <imgui/imgui.h>

namespace OpenGLFun {
	namespace TextureUtils {
		ImTextureID GetImGuiTexId(std::string const& textureName);
	}
}