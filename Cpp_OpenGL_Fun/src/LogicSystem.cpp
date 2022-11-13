#include "LogicSystem.h"

#include <mono/jit/jit.h>

#include "Engine.h"

namespace OpenGLFun {
	LogicSystem* LOGIC_SYSTEM = nullptr;

	void GetAllBehaviours(std::vector<std::string>& list, MonoImage* image);

	LogicSystem::LogicSystem() : _monoApiAssembly{ nullptr }, _monoApiImage{ nullptr }, _monoAppAssembly{ nullptr }, _monoAppImage{ nullptr } {
		if (LOGIC_SYSTEM)
			throw SimpleException("Logic system already created!");

		LOGIC_SYSTEM = this;

		_monoApiAssembly = mono_domain_assembly_open(ENGINE->GetMonoDomain(), ".\\scripts\\OpenGLFunScriptAPI.dll");
		if (!_monoApiAssembly)
			throw SimpleException("Could not open OpenGLFunScriptAPI.dll");

		_monoApiImage = mono_assembly_get_image(_monoApiAssembly);
		if (!_monoApiImage)
			throw SimpleException("Could not get image from OpenGLFunScriptAPI.dll");

		_monoAppAssembly = mono_domain_assembly_open(ENGINE->GetMonoDomain(), ".\\scripts\\OpenGLFunScripts.dll");
		if (!_monoAppAssembly)
			throw SimpleException("Could not open OpenGLFunScripts.dll");

		_monoAppImage = mono_assembly_get_image(_monoAppAssembly);
		if (!_monoAppImage)
			throw SimpleException("Could not get image from OpenGLFunScripts.dll");

		GetAllBehaviours(_behaviourList, _monoAppImage);
	}

	LogicSystem::~LogicSystem() {}

	void LogicSystem::Update(float const& /*deltaTime*/) {

	}

	const std::vector<std::string>& LogicSystem::GetBehaviourList() const {
		return _behaviourList;
	}

	void GetAllBehaviours(std::vector<std::string>& list, MonoImage* image) {
		list.clear();

		// code from: https://stackoverflow.com/a/47510808
		const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(table_info);

		for (int i = 0; i < rows; i++) {
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			std::string className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string classNamespace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);

			if (classNamespace.empty())
				continue;
			list.push_back(classNamespace + "." + className);
		}
	}
}