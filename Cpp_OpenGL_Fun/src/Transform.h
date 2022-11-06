#pragma once
#include "Component.h"
#include "Vec3f.h"

namespace OpenGLFun {
	class Transform : public IComponent {
	public:
		Vec3f mPositionOld;
		Vec3f mPosition;
		Vec3f mRotationOld;
		Vec3f mRotation;
		Vec3f mScale;

		Transform();
		Transform(EntityId const& owner, Vec3f pos, Vec3f rot, Vec3f scale);
		~Transform() override;

		void Deserialize(rapidjson::Value const& jsonObj) override;

		bool DrawImGuiComponent() override;
	};
}