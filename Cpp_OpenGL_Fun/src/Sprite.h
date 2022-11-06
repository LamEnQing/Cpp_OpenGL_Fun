#pragma once
#include "pch.h"
#include "Component.h"
#include "Vec2.h"

namespace OpenGLFun {
	class Sprite : public IComponent {
	private:
		int _selectedTexture = 0;
	public:
		std::string mTextureFilepath;
		Vec2<int> mUVPosition;
		Vec2<int> mUVDimensions;

		Sprite();
		~Sprite() override;

		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}
