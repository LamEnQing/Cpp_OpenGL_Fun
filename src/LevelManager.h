#pragma once
#include "pch.h"

namespace OpenGLFun {
	class LevelManager {
	public:
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