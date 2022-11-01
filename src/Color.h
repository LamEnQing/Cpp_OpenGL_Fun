#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec4f.h"

namespace OpenGLFun {
	class Color : public IComponent {
	public:
		Vec4f mRgba;

		Color();
		~Color() override;

		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}