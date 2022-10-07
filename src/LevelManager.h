#pragma once
#include "pch.h"
#include "EntityManager.h"

namespace OpenGLFun {
	class LevelManager {
	public:
		//std::map<std::string, EntityId> mLoadedLevelEntities;
		std::string mCurrentLevel;

		LevelManager();
		~LevelManager();

		// read the level names and save them to a vector of strings
		void Load();
		// clears the vector of strings, containing level names
		void Unload();
		// clears the resources, entities and components. And load new data for the current level
		void ReloadLevel();

		void LoadLevel(std::string const& levelId);

		int MaxLevel() const;
	private:
		std::vector<std::string> _levels;
	};

	extern std::unique_ptr<LevelManager> LEVEL_MANAGER;
}