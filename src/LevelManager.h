#pragma once
#include "pch.h"
#include "EntityManager.h"

namespace OpenGLFun {
	class LevelManager {
	public:
		//std::map<std::string, EntityId> mLoadedLevelEntities;

		LevelManager();
		~LevelManager();

		void Load();
		void Unload();

		void LoadLevel(std::string const& levelId);

		int MaxLevel() const;
	private:
		int _currentLevel;
		std::vector<std::string> _levels;
	};

	extern std::unique_ptr<LevelManager> LEVEL_MANAGER;
}