#pragma once
#include <bitset>

namespace OpenGLFun {
	// decides which shader and draw function to use
	enum class ModelType : int {
		TwoD, ThreeD
	};

	enum class ComponentType : int {
		Camera,

		Button,
		Color,
		Model,
		Sprite,
		Transform,

		MAXIMUM
	};

	using EntityId = int;
	const EntityId MAX_ENTITIES = 1000;
	using Signature = std::bitset<static_cast<int>(ComponentType::MAXIMUM)>;
}