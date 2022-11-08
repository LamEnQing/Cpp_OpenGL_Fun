#pragma once
#include "pch.h"
#include "EntityManager.h"

namespace OpenGLFun {
	class LevelManager {
	public:
		bool mShouldReloadLevel; // should reload level or not
		std::string mCurrentLevel; // current level's name

		LevelManager();
		~LevelManager();

		// clears the resources, entities and components. And load new data for the current level
		void ReloadLevel();

		void LoadLevel(std::string const& levelId);
	private:
	};

	extern std::unique_ptr<LevelManager> LEVEL_MANAGER;
	extern const std::string LEVEL_DIR;
}