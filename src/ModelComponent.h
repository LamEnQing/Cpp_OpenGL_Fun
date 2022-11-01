#pragma once
#include "Component.h"
#include "Types.h"

namespace OpenGLFun {
	class ModelComponent : public IComponent {
	public:
		ModelType mModelType;
		std::string mModelFilepath;
		bool mEnableBlend;
		bool mShouldCull;

		ModelComponent();
		ModelComponent(EntityId const& owner);
		~ModelComponent() override;

		// Throws an exception if string is not either '2d' or '3d'. You MUST catch the exception and throw your own error message exception
		static ModelType ParseModelTypeString(std::string typeStr);

		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}