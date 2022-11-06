#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec2.h"
#include "Vec4f.h"

namespace OpenGLFun {
	class Button : public IComponent {
	public:
		Vec4f mHoverRgba;
		Vec2<int> mHoverUVPos;
		Vec2<int> mHoverUVDimensions;

		Button();
		~Button() override;
		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}