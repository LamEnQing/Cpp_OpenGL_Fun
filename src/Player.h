#pragma once
#include "Entity.h"

namespace OpenGLFun {
	struct Player : Entity {
		// for camera purposes, to form lookAt vector
		float headYRotOld; // or yaw, rotates around the y-axis
		float headYRot;
		float headXRotOld; // or pitch, rotates around the x-axis
		float headXRot;

		float movementSpeed;
	};
}