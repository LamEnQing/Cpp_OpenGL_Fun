#include "Behaviour.h"

#include "LogicSystem.h"

namespace OpenGLFun {
	Behaviour::Behaviour() {
		mCompType = ComponentType::Behaviour;
	}

	Behaviour::~Behaviour() {}

	void Behaviour::Deserialize(rapidjson::Value const& jsonObj) {}

	bool Behaviour::DrawImGuiComponent() {
		bool canClose = true;
		if (!ImGui::CollapsingHeader("Behaviour", &canClose)) return !canClose;

		/*const auto& behaviourList = LOGIC_SYSTEM->GetBehaviourList();

		if (behaviourList.size() == 0)
			_selectedBehaviour = -1;

		if (ImGui::BeginCombo("##behaviour select list", _selectedBehaviour == -1 ? "Select a behaviour" : behaviourList[_selectedBehaviour].c_str())) {
			for (size_t i = 0; i < behaviourList.size(); i++) {
				if (ImGui::Selectable(behaviourList[i].c_str())) {
					_selectedBehaviour = i;
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		ImGui::BeginDisabled(_selectedBehaviour < 0);
		if (ImGui::Button("Add Behaviour script")) {
			auto insertResult = _behaviourClasspaths.insert(behaviourList[_selectedBehaviour]);
			if (insertResult.second) { // second is a boolean that is true if new element was inserted
				// Call LogicSystem to add a new behaviour
			}
		}
		ImGui::EndDisabled();

		if (ImGui::BeginListBox("##behaviour list", { -FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing() })) {
			for (const std::string& classPath : _behaviourClasspaths) {
				ImGui::Selectable(classPath.c_str());
			}
			ImGui::EndListBox();
		}*/

		return !canClose;
	}
}