#pragma once

#include "Component.h"

namespace OpenGLFun {
	class Behaviour : public IComponent {
	public:
		Behaviour();
		~Behaviour() override;

		void Deserialize(rapidjson::Value const& jsonObj) override;
		bool DrawImGuiComponent() override;
	private:
		int _selectedBehaviour = -1;

		std::set<std::string> _behaviourClasspaths;
	};
}