#pragma once
#include "pch.h"
#include "EntityManager.h"

namespace OpenGLFun {
	class LevelManager {
	public:
		std::vector<EntityId> mPauseScreenObjs;

		LevelManager();
		~LevelManager();

		void Load();
		void Unload();
	private:
		int _currentLevel;
		int _maxLevel;
	};

	extern std::unique_ptr<LevelManager> LEVEL_MANAGER;
}