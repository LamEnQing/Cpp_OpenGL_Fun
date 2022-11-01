#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec4f.h"

namespace OpenGLFun {
	class Button : public IComponent {
	public:
		Vec4f mHoverRgba;
		std::array<int, 2> mHoverUVPos;
		std::array<int, 2> mHoverUVDimensions;

		Button();
		~Button() override;
		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}