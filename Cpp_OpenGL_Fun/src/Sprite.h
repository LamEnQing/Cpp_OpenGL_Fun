#pragma once
#include "pch.h"
#include "Component.h"

namespace OpenGLFun {
	class Sprite : public IComponent {
	private:
		int _selectedTexture = 0;
	public:
		std::string mTextureFilepath;
		std::array<int, 2> mUVPosition;
		std::array<int, 2> mUVDimensions;

		Sprite();
		~Sprite() override;

		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}
